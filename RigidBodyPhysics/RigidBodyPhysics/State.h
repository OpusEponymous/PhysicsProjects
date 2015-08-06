#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef STATE_H
#define STATE_H

using namespace std;

class State
{
public:
	//Primary Variables
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 linearMomentum;
	glm::vec3 angularMomentum;

	//Secondary Variables
	glm::vec3 linearVelocity;
	glm::quat spin;
	glm::vec3 angularVelocity;

	glm::mat4 modelMatrix;
	glm::mat4 invModelMatrix;

	//Constants
	float size;
	float mass;
	float inverseMass;
	glm::mat3 inertiaTensor;
	glm::mat3 inverseInertiaTensor;

	State();
	State(float l, float m);
	State(glm::mat3 iTensor, float m);

	void recalculate();



};

#endif