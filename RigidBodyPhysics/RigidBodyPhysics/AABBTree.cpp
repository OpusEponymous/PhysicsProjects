#include "AABBTree.h"

AABBTree::AABBTree()
{
	root = nullptr;
	margin = 0.2f;
}

//Node Insertion

void AABBTree::Add(RigidBody *rBody)
{
	if(root)
	{
		//Then not the first not, insert into tree
		Node *node =  new Node();
		node->SetLeaf(&(rBody->boundingBox));
		node->UpdateAABB(margin);
		InsertNode(node, &root);
	}
	else
	{
		//Create root node
		root = new Node();
		root->SetLeaf(&(rBody->boundingBox));
		root->UpdateAABB(margin);
	}
}

void AABBTree::InsertNode(Node *node, Node **parent)
{
	Node *p = *parent;
	if(p->IsLeaf())
	{
		//Parent is a Leaf, split
		Node *newParent = new Node();
		newParent->parent = p->parent;
		newParent->SetBranch(node, p);
		*parent = newParent;
	}
	else
	{
		//Parent is branch, compute volume differences fr pre/post insert
		AABB aabb0 = p->children[0]->aabb;
		AABB aabb1 = p->children[1]->aabb;
		float volumeDiff0 = aabb0.Union(node->aabb).Volume() - aabb0.Volume();
		float volumeDiff1 = aabb1.Union(node->aabb).Volume() - aabb1.Volume();

		//insert to the child that gives less volume increases
		if(volumeDiff0 < volumeDiff1)
		{
			InsertNode(node, &p->children[0]);
		}
		else
		{
			InsertNode(node, &p->children[1]);
		}

		//Update arent AABB - Recursive
		(*parent)->UpdateAABB(margin);

	}
}

//Node Removal
void AABBTree::Remove(RigidBody *rBody)
{
	Node *node = rBody->boundingBox.userData;

	// remove two-way link
	node->data = nullptr;
	rBody->boundingBox.userData = nullptr;

	RemoveNode(node);
}

void AABBTree::RemoveNode(Node *node)
{
	//Replace parent with sibling, remove parent node
	Node *parent = node->parent;
	if(parent)
	{
		Node *sibling = node->GetSibling();
		if(parent->parent)
		{
			//If there is a grandparent then update links
			sibling->parent = parent->parent;
			(parent == parent->parent->children[0]
				? parent->parent->children[0]
				: parent->parent->children[1]) = sibling;
		}
		else
		{
			//If there is no grandparent we need to change the root
			Node *sibling = node->GetSibling();
			root = sibling;
			sibling->parent = nullptr;
		}

		delete node;
		delete parent;
	}
	else
	{
		//If the node is root
		root = nullptr;
		delete node;
	}
}

//Update Functions

void AABBTree::Update()
{
	if(root)
	{
		if(root->IsLeaf())
		{
			root->UpdateAABB(margin);
		}
		else
		{
			//Gather invalid Nodes
			invalidNodes.clear();
			UpdateNodeHelper(root, invalidNodes);
			
			//Reinsert all invalid Nodes
			for(vector<Node *>::iterator i = invalidNodes.begin(); i!=invalidNodes.end(); ++i)
			{
				Node *node = *i;
				Node *parent = node->parent;
				Node *sibling = node->GetSibling();
				Node **parentLink =
					parent->parent 
						? (parent == parent->children[0]
							? &parent->parent->children[0]
							: &parent->parent->children[1])
						: &root;

				//Replace parent with sibling
				sibling->parent = 
					parent->parent
						? parent->parent
						: nullptr;

				*parentLink = sibling;
				delete parent;

				//reinsert node
				node->UpdateAABB(margin);
				InsertNode(node, &root);
			}
			invalidNodes.clear();
		}
	}
}

void AABBTree::UpdateNodeHelper(Node *node, vector<Node *> &invalidNodes)
{
	if(node->IsLeaf())
	{
		//Check if the Fat AABB doesnt contain the collider anymore
		if(!node->aabb.Contains(*(node->data)))
		{
			invalidNodes.push_back(node);
		}
		else
		{
			UpdateNodeHelper(node->children[0], invalidNodes);
			UpdateNodeHelper(node->children[1], invalidNodes);
		}
	}
}

//Broadphase Functions

vector<pair<RigidBody *, RigidBody *>> &AABBTree::ComputePairs()
{
	pairs.clear();

	//Early Out if empty or only 1 rigid body
	if(!root || root->IsLeaf())
	{
		return pairs;
	}

	ClearChildrenCrossFlagHelper(root);
	
	ComputePairsHelper(root->children[0], root->children[1]);

	return pairs;
}

void AABBTree::ClearChildrenCrossFlagHelper(Node *node)
{
	node->childrenCrossed = false;
	if(!node->IsLeaf())
	{
		ClearChildrenCrossFlagHelper(node->children[0]);
		ClearChildrenCrossFlagHelper(node->children[1]);
	}
}

void AABBTree::CrossChildren(Node *node)
{
	if(!node->childrenCrossed)
	{
		ComputePairsHelper(node->children[0], node->children[1]);
		node->childrenCrossed = true;
	}
}

void AABBTree::ComputePairsHelper(Node *n0, Node *n1)
{
	if(n0->IsLeaf())
	{
		if(n1->IsLeaf())
		{
			//2 Leaves, Check Proxies
			if(n0->data->checkCollision(*n1->data))
			{
				pair<AABB *, AABB *> p(n0->data, n1->data);		//Possible Error Source
				pairs.push_back(p);
			}
		}
		else
		{
			//1 Branch 1 Leaf
			CrossChildren(n1);
			ComputePairsHelper(n0->children[0], n1);
			ComputePairsHelper(n1->children[1], n0);
		}
	}
	else
	{
		if(n1->IsLeaf())
		{
			//1 Branch 1 Leaf
			CrossChildren(n0);
			ComputePairsHelper(n0->children[0], n1);
			ComputePairsHelper(n0->children[1], n1);
		}
		else
		{
			//2 branches
			CrossChildren(n0);
			CrossChildren(n1);
			ComputePairsHelper(n0->children[0], n1->children[0]);
			ComputePairsHelper(n0->children[0], n1->children[1]);
			ComputePairsHelper(n0->children[1], n1->children[0]);
			ComputePairsHelper(n0->children[1], n1->children[1]);
		}
	}

}