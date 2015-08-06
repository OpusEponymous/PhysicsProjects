#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef DERIVATIVE_H
#define DERIVATIVE_H

using namespace std;

class Derivative
{
public:
	glm::vec3 velocity;
	glm::quat spin;
	glm::vec3 force;
	glm::vec3 torque;

	Derivative();
	Derivative(glm::vec3 vel, glm::quat spin, glm::vec3 f, glm::vec3 tor);
};

#endif