#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Ray.h"

#ifndef AABB_H
#define AABB_H

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

struct Node; // #Believe

class AABB
{
public:
	float maxX, maxY, maxZ;
	float minX, minY, minZ;
	
	Node *userData;
	
	AABB();
	AABB(float mX, float mY, float mZ, float mnX, float mnY, float mnZ);
	
	bool isBroadColliding;	//Debug Purposes only
	bool isNarrowColliding;	//Debug Purposes only

	void Draw();
	void Update(float mX, float mY, float mZ, float mnX, float mnY, float mnZ);
	void setBroadColliding(bool state);

	bool checkCollision(AABB &b);
	bool Contains(glm::vec3 pt);
	bool Contains(AABB &b);
	bool TestRay(Ray3 ray);

	float Volume();

	AABB Union(AABB aabb);
private:
	vector<glm::vec3> vertexPositions;
	vector<glm::vec4> vertexColors;
	GLuint vertexIndices[48];

	GLuint vertexArraySize;
	GLuint colorArraySize;

	unsigned int numIndices;

	GLuint VBO;
	GLuint VAO;
	GLuint IB;

	void generateObjectBuffer();
	void generateVertices();
};

#endif