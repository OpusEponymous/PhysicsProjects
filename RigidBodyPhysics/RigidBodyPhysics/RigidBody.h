#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ConvexMesh.h"
#include "Derivative.h"
#include "State.h"
#include "Common.h"
#include "AABB.h"

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

using namespace std;


class RigidBody
{
public:
	glm::vec3 origin;
	int ID;
	State currentState;
	State previousState;
	glm::vec3 springPos;
	bool isBroadColliding;
	bool isNarrowColliding;
	bool hasSpring;
	vector<glm::vec3> vertexPositions;
	//Cube cube;
	ConvexMesh collider;
	AABB boundingBox;
	
	RigidBody();
	RigidBody(float length, float mass, int id);
	RigidBody(float mass, int id, vector<glm::vec3> &vPositions, vector<glm::vec4> &vColors, vector<glm::vec3> &vNormals, vector<GLuint> &vIndices);

	void Update(float t, float dt);
	void Draw();
	void calcBoundingBox();
	void applyTransform(State state);
	void setBroadColliding(bool state);
	void setNarrowColliding(bool state);
	void translateGeometry(glm::vec3 trans);
	void rotateGeometry(glm::quat rot);
	void attachSpring(glm::vec3 sPos);
	void detachSpring();

	AABB *GetBoundingBox();

private:
	float k;
	float b;

	vector<glm::vec3> originalVertexPositions;
	
	void generateVertices(float length);
	Derivative Evaluate(const State &initial, float t, float dt, const Derivative &d);
	void Force(const State &state, Derivative &derivative, float t);
	void Torque(const State &state, Derivative &derivative, float t);
	void Integrate(State &state, float t, float dt);

};

#endif