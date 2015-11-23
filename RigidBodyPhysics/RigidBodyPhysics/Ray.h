#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef RAY_H
#define RAY_H

struct Ray3
{
	glm::vec3 pos;
	glm::vec3 dir;
};

#endif