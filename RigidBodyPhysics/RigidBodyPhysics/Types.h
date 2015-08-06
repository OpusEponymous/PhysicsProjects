#include "RigidBody.h"

#ifndef TYPES_H
#define TYPES_H

struct RayCastResult
{
	bool hit;
	RigidBody *collider;
	glm::vec3 position;
	glm::vec3 normal;
};

#endif