#include "AABBTree.h"

//Default Constructor
AABBTree::AABBTree()
{
	root = nullptr;
	margin = 0.2f;
}

//Node Insertion
void AABBTree::Add(RigidBody *rBody)
{
	if(root)	//If not the first node
	{
		Node *node = new Node();
		node->SetLeaf(rBody);
		node->UpdateAABB(margin);
		InsertNode(node, &root);
	}
	else
	{
		root = new Node();
		root->SetLeaf(rBody);
		root->UpdateAABB(margin);
	}

}

void AABBTree::InsertNode(Node *node, Node **parent)
{
	Node *p = *parent;
	if(p->IsLeaf())
	{
		//Parent is a leaf so split
		Node *newParent = new Node();
		newParent->parent = p->parent;
		newParent->SetBranch(node, p);
		*parent = newParent;
	}
	else
	{
		//Parent is a branch, compute volume heuristic
		AABB aabb0 = p->children[0]->aabb;
		AABB aabb1 = p->children[1]->aabb;
		const float volumeDiff0 = aabb0.Union(node->aabb).Volume() - aabb0.Volume();
		const float volumeDiff1 = aabb1.Union(node->aabb).Volume() - aabb1.Volume();

		//Insert to the child that gives less volume increase
		if(volumeDiff0 < volumeDiff1)
		{
			InsertNode(node, &p->children[0]);
		}
		else
		{
			InsertNode(node, &p->children[1]);
		}

	}
	//Update parent AABB (Propagates back up the stack)
	(*parent)->UpdateAABB(margin);
}

//Update AABBs
void AABBTree::Update()
{
	if(root)	//If the root is not null
	{
		if(root->IsLeaf())	//If the root is the only node in the tree
		{
			root->UpdateAABB(margin);
		}
		else	//If there is more than one node in the tree
		{
			//Find the invalid nodes
			invalidNodes.clear();
			UpdateNodeHelper(root, invalidNodes);

			//Re-insert all invalid nodes
			for(Node *node: invalidNodes)
			{
				//Get the parent link (pointer to the pointer to the parnet)
				Node *parent = node->parent;
				Node *sibling = node->GetSibling();
				
				// I hate these experssions must change them
				Node **parentLink =
					parent->parent
						? (parent == parent->parent->children[0]
							? &parent->parent->children[0]
							: &parent->parent->children[1])
						: &root;
				
				//Replace parent with sibling
				sibling->parent =
					parent->parent
						? parent->parent
						: nullptr;	//root has null parent

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
		//Check if the fat AABB doesn't contain the collider's AABB
		if(!node->aabb.Contains(node->data->boundingBox))
		{
			invalidNodes.push_back(node);
		}
	}
	else
	{
		UpdateNodeHelper(node->children[0], invalidNodes);
		UpdateNodeHelper(node->children[1], invalidNodes);
	}
}

//Node Removal
void AABBTree::Remove(RigidBody *rBody)
{
	Node *node = static_cast<Node *>(rBody->userData);

	//Remove two-way link
	node->data = nullptr;
	rBody->userData = nullptr;

	RemoveNode(node);
}

void AABBTree::RemoveNode(Node *node)
{
	//Replace parent with sibling, remove parent node
	Node *parent = node->parent;
	if(parent)
	{
		Node *sibling = node->GetSibling();
		if(parent->parent)	//if the node has a grandparent
		{
			//Update links
			sibling->parent = parent->parent;
			(parent == parent->parent->children[0]
				? parent->parent->children[0]
				: parent->parent->children[1]) = sibling;
		}
		else	//If the node has no grandparent
		{
			//make the sibling root
			Node *sibling = node->GetSibling();
			root = sibling;
			sibling->parent = nullptr;
		}

		delete node;
		delete parent;
	}
	else	//Node is Root
	{
		root = nullptr;
		delete node;
	}




}

//Compute Colliding Pairs
vector<pair<RigidBody *, RigidBody *>> &AABBTree::ComputePairs()
{
	pairs.clear();

	//Early Out - If the tree is empty or only has one node
	if(!root || root->IsLeaf())
	{
		return pairs;
	}

	//Clear the childrenCrossed flags in the node class
	ClearChildrenCrossFlagHelper(root);

	// Initial recursive call
	ComputePairsHelper(root->children[0], root->children[1]);

	return pairs;
}

void AABBTree::ClearChildrenCrossFlagHelper(Node *node)
{
	node->childrenCrossed = false;
	//If it has children clear their flags recursively
	if( !node->IsLeaf())
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
			//2 Leaves so check AABB
			if(n0->data->boundingBox.checkCollision(n1->data->boundingBox))
			{
				pair<RigidBody*, RigidBody*> p(n0->data, n1->data);
				pairs.push_back(p);
			}
		}
		else
		{
			//1 Leaf 1 Branch, 2 Cross Check (n0 against children of n1)
			CrossChildren(n1);
			ComputePairsHelper(n0, n1->children[0]);
			ComputePairsHelper(n0, n1->children[1]);
		}
	}
	else
	{
		if(n1->IsLeaf())
		{
			//1 Leaf 1 Branch, 2 Cross Check (n1 against children of n0)
			CrossChildren(n0);
			ComputePairsHelper(n0->children[0], n1);
			ComputePairsHelper(n0->children[1], n1);
		}
		else
		{
			//2 Branches, 4 Cross Checks
			CrossChildren(n0);
			CrossChildren(n1);
			ComputePairsHelper(n0->children[0], n1->children[0]);
			ComputePairsHelper(n0->children[0], n1->children[1]);
			ComputePairsHelper(n0->children[1], n1->children[0]);
			ComputePairsHelper(n0->children[1], n1->children[1]);

		}
	}
}
//Pick

RigidBody *AABBTree::Pick(const glm::vec3 &point) const
{
	//To Do
	return nullptr;
}

//Query



//RayCast

RayCastResult AABBTree::RayCast(const Ray3 &ray) const{
	//To Do
	RayCastResult a;
	return a;
}