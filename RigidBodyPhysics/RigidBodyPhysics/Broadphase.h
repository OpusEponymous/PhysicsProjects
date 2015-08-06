#include "Types.h"

#ifndef BROADPHASE_H
#define BROADPHASE_H

using namespace std;


typedef pair<RigidBody *, RigidBody *> ColliderPair;

class Broadphase
{
public:

	//Add a new AABB
	virtual void Add(RigidBody *rBody) = 0;	
	
	//updates broadphase to react to changes in AABBs
	virtual void Update() = 0;

	//return a list of possibly collining pairs
	virtual const vector<pair<RigidBody *, RigidBody *>> &ComputePairs() = 0;

	//return a collider colliding with a target point, null if no such collider
	virtual RigidBody *Pick( const glm::vec3 &point) const = 0;

	//Return vector of colliders whose AABBs collide with a target AABB
	virtual void Query(AABB &aabb, vector<RigidBody *> &output) const = 0;

	//Result contains the first collider the ray hits
	//Result contains null if no collider is
	virtual RayCastResult RayCast(const Ray3 &ray) const = 0;
};

#endif