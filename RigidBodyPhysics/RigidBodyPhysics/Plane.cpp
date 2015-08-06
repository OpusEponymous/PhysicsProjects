#include "Plane.h"

Plane::Plane(){
	normal = glm::vec3(0.0f, 0.0f, 0.0f);
	point = glm::vec3(0.0f, 0.0f, 0.0f);
}

Plane::Plane(glm::vec3 norm, glm::vec3 pnt){
	normal = norm;
	point = pnt;
}