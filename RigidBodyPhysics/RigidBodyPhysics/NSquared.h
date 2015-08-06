#include "Types.h"

#ifndef NSQUARED_H
#define NSQUARED_H

class NSquared 
{
public:
	NSquared();
	void Add(RigidBody *rBody);
	void Update();
	vector<pair<RigidBody *, RigidBody *>> &ComputePairs();
	RigidBody *Pick( const glm::vec3 &point);
	void Query(AABB &aabb, vector<RigidBody *> &output);
	RayCastResult RayCast(const Ray3 &ray);

private:
	vector<RigidBody *> ColliderList;
	vector<pair<RigidBody *, RigidBody *>> ColliderPairList;
};

#endif