#include "Node.h"

#ifndef AABBTREE_H
#define AABBTREE_H

class AABBTree
{
public:
	AABBTree();
	void Add(RigidBody *rBody);
	void Remove(RigidBody *rBody);
	void Update();
	vector<pair<RigidBody *, RigidBody *>> &ComputePairs();
	RigidBody *Pick(const glm::vec3 &point) const;
	//void Query(const RigidBody &rBody, vector<RigidBody *> &output) const;	//Not Sure I need/want this
	RayCastResult RayCast(const Ray3 &ray) const;

private:
	void UpdateNodeHelper(Node *node, vector<Node *> &invalidNodes);
	void InsertNode(Node *node, Node **parent);
	void RemoveNode(Node *node);
	void ComputePairsHelper(Node *n0, Node* n1);
	void ClearChildrenCrossFlagHelper(Node *node);
	void CrossChildren(Node *node);

	Node *root;
	vector<pair<RigidBody *, RigidBody *>> pairs;
	float margin;
	vector<Node *> invalidNodes;

};

#endif // !AABBTREE_H