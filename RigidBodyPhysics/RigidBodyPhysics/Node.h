#include "Types.h"

struct Node
{
	Node *parent;
	Node *children[2];

	bool childrenCrossed;
	AABB aabb;
	RigidBody *data;
	
	Node()
	{
		parent = nullptr;
		data = nullptr;
		children[0] = nullptr;
		children[1] = nullptr;
	}

	bool IsLeaf()
	{
		return children[0] == nullptr;
	}

	void SetBranch(Node *n0, Node *n1)
	{
		n0->parent = this;
		n1->parent = this;

		children[0] = n0;
		children[1] = n1;
	}

	void SetLeaf(RigidBody *data)
	{
		this->data = data;
		data->userData = this;

		children[0] = nullptr;
		children[1] = nullptr;
	}

	void UpdateAABB(float margin)
	{
		if(IsLeaf())
		{
			//Make Fat AABB
			aabb.minX = data->boundingBox.minX - margin;
			aabb.minY = data->boundingBox.minY - margin;
			aabb.minZ = data->boundingBox.minZ - margin;

			aabb.maxX = data->boundingBox.maxX + margin;
			aabb.maxY = data->boundingBox.maxY + margin;
			aabb.maxZ = data->boundingBox.maxZ + margin;
		}
		else
		{
			//make union of all child nodes
			aabb = children[0]->aabb.Union(children[1]->aabb);
		}
	}

	Node *GetSibling()
	{
		return 
			this == parent->children[0]
				? parent->children[1]
				: parent->children[0];
	}
	
};