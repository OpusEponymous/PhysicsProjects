#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef PLANE_H
#define PLANE_H

using namespace std;

class Plane{
public:
	glm::vec3 normal;
	glm::vec3 point;

	Plane();
	Plane(glm::vec3 norm, glm::vec3 pnt);
};

#endif