#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#ifndef CONVEXMESH_H
#define CONVEXMESH_H

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

class ConvexMesh
{
public:
	ConvexMesh();
	ConvexMesh(int type, float length);
	ConvexMesh(vector<glm::vec3> &vPositions, vector<glm::vec4> &vColors, vector<glm::vec3> &vNormals, vector<GLuint> &vIndices);
	void Draw();

	vector<glm::vec3> vertexPositions;
	vector<glm::vec4> vertexColors;
	vector<glm::vec3> vertexNormals;
	vector<GLuint> vertexIndices;

	void GeneratePyramid(float length);
	void GenerateTetrahedron(float baseLength, float height);
	void GenerateCube(float length);

private:	

	GLuint vertexArraySize;
	GLuint colorArraySize;
	GLuint normalArraySize;
	unsigned int numIndices;
	
	GLuint VBO;
	GLuint VAO;
	GLuint IB;

	void generateObjectBuffer();

};


#endif