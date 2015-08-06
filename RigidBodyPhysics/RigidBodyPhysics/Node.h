#include "Types.h"

struct Node
{
	Node *parent;
	Node *children[2];

	bool childrenCrossed;

	AABB aabb;
	AABB *data;

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

	void SetLeaf(AABB *data)
	{
		this->data = data;
		data->userData = this;

		children[0] = nullptr;
		children[1] = nullptr;
	}
	
	void SetBranch(Node *n0, Node *n1)
	{
		n0->parent = this;
		n1->parent = this;

		children[0] = n0;
		children[1] = n1;
	}

	void UpdateAABB(float margin)
	{
		if(IsLeaf())
		{
			//Make Fat AABB
			aabb.minX = data->minX - margin;
			aabb.minY = data->minY - margin;
			aabb.minZ = data->minZ - margin;

			aabb.maxX = data->maxX + margin;
			aabb.maxY = data->maxY + margin;
			aabb.maxZ = data->maxZ + margin;
		}
		else
		{
			//make union of all child nodes
			aabb = children[0]->aabb.Union(children[1]->aabb);
		}
	}

	Node *GetSibling()
	{
		if(this == parent->children[0])
		{
			return parent->children[1];
		}
		return parent->children[0];
	}

};