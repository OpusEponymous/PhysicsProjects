#include "ConvexMesh.h"

ConvexMesh::ConvexMesh()
{
	//Default Constructor
}

ConvexMesh::ConvexMesh(int type, float length)
{
	//Not elegant but it will do for now
	if(type == 0)	//Cube
	{
		GenerateCube(length);		
	}
	else			//Pyramid
	{
		GeneratePyramid(length);		
	}
	generateObjectBuffer();
}

ConvexMesh::ConvexMesh(vector<glm::vec3> &vPositions, vector<glm::vec4> &vColors, vector<glm::vec3> &vNormals, vector<GLuint> &vIndices)
{
	int pSize = vPositions.capacity();
	int cSize = vColors.capacity();
	int nSize = vNormals.capacity();
	int iSize = vIndices.capacity();

	vertexPositions.resize(pSize);
	vertexColors.resize(cSize);
	vertexNormals.resize(nSize);
	vertexIndices.resize(iSize);

	for(int i=0; i<pSize; i++)
	{
		vertexPositions[i] = vPositions[i];
	}

	for(int i=0; i<cSize; i++)
	{
		vertexColors[i] = vColors[i];
	}

	for(int i=0; i<nSize; i++)
	{
		vertexNormals[i] = vNormals[i];
	}

	for(int i=0; i<iSize; i++)
	{
		vertexIndices[i] = vIndices[i];
	}
}

void ConvexMesh::Draw()
{
	//Render the object
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void ConvexMesh::GeneratePyramid(float length)
{
	//create pyramid mesh
}

void ConvexMesh::GenerateTetrahedron(float baseLength, float height)
{
	//Create terahedral mesh


}
void ConvexMesh::GenerateCube(float length)
{
	//Create cube mesh

	float dist = length/2.0f;

	//Colours
	glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 blue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 yellow = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 purple = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 cyan = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 orange = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
	glm::vec4 white = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	//Normals
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 left = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 back = glm::vec3(0.0f, 0.0f, -1.0f);

	//FrontFace
	vertexPositions.push_back(glm::vec3(-dist, dist, dist));	vertexColors.push_back(red);	vertexNormals.push_back(front);		//0
	vertexPositions.push_back(glm::vec3(dist, dist, dist));		vertexColors.push_back(red);	vertexNormals.push_back(front);		//1
	vertexPositions.push_back(glm::vec3(dist, -dist, dist));	vertexColors.push_back(red);	vertexNormals.push_back(front);		//2
	vertexPositions.push_back(glm::vec3(-dist, -dist, dist));	vertexColors.push_back(red);	vertexNormals.push_back(front);		//3

	//RightFace
	vertexPositions.push_back(glm::vec3(dist, dist, dist));		vertexColors.push_back(green);	vertexNormals.push_back(right);		//4
	vertexPositions.push_back(glm::vec3(dist, dist, -dist));	vertexColors.push_back(green);	vertexNormals.push_back(right);		//5
	vertexPositions.push_back(glm::vec3(dist, -dist, -dist));	vertexColors.push_back(green);	vertexNormals.push_back(right);		//6
	vertexPositions.push_back(glm::vec3(dist, -dist, dist));	vertexColors.push_back(green);	vertexNormals.push_back(right);		//7

	//BackFace
	vertexPositions.push_back(glm::vec3(dist, dist, -dist));	vertexColors.push_back(blue);	vertexNormals.push_back(back);		//8
	vertexPositions.push_back(glm::vec3(-dist, dist, -dist));	vertexColors.push_back(blue);	vertexNormals.push_back(back);		//9
	vertexPositions.push_back(glm::vec3(-dist, -dist, -dist));	vertexColors.push_back(blue);	vertexNormals.push_back(back);		//10
	vertexPositions.push_back(glm::vec3(dist, -dist, -dist));	vertexColors.push_back(blue);	vertexNormals.push_back(back);		//11

	//LeftFace
	vertexPositions.push_back(glm::vec3(-dist, dist, -dist));	vertexColors.push_back(purple);	vertexNormals.push_back(left);		//12
	vertexPositions.push_back(glm::vec3(-dist, dist, dist));	vertexColors.push_back(purple);	vertexNormals.push_back(left);		//13
	vertexPositions.push_back(glm::vec3(-dist, -dist, dist));	vertexColors.push_back(purple);	vertexNormals.push_back(left);		//14
	vertexPositions.push_back(glm::vec3(-dist, -dist, -dist));	vertexColors.push_back(purple);	vertexNormals.push_back(left);		//15

	//TopFace
	vertexPositions.push_back(glm::vec3(-dist, dist, -dist));	vertexColors.push_back(yellow);	vertexNormals.push_back(up);		//16
	vertexPositions.push_back(glm::vec3(dist, dist, -dist));	vertexColors.push_back(yellow);	vertexNormals.push_back(up);		//17
	vertexPositions.push_back(glm::vec3(dist, dist, dist));		vertexColors.push_back(yellow);	vertexNormals.push_back(up);		//18
	vertexPositions.push_back(glm::vec3(-dist, dist, dist));	vertexColors.push_back(yellow);	vertexNormals.push_back(up);		//19

	//BottomFace
	vertexPositions.push_back(glm::vec3(-dist, -dist, dist));	vertexColors.push_back(orange);	vertexNormals.push_back(down);		//20
	vertexPositions.push_back(glm::vec3(dist, -dist, dist));	vertexColors.push_back(orange);	vertexNormals.push_back(down);		//21
	vertexPositions.push_back(glm::vec3(dist, -dist, -dist));	vertexColors.push_back(orange);	vertexNormals.push_back(down);		//22
	vertexPositions.push_back(glm::vec3(-dist, -dist, -dist));	vertexColors.push_back(orange);	vertexNormals.push_back(down);		//23

	//CreateIndices
	GLuint indices[] = { 
		0, 1, 2,
		2, 3, 0,
	
		4, 5, 6,
		6, 7, 4,
	
		8, 9, 10,
		10, 11, 8,
	
		12, 13, 14,
		14, 15, 12,
	
		16, 17, 18,
		18, 19, 16,
	
		20, 21, 22,
		22, 23, 20

	};

	for(int i = 0; i<36; i++){
		vertexIndices.push_back(indices[i]);
	}
	numIndices = 36;
	cout << "Test Print";
}

void ConvexMesh::generateObjectBuffer()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	numIndices = vertexIndices.size();
	vertexArraySize = vertexPositions.size()*sizeof(glm::vec3);
	colorArraySize = vertexColors.size()*sizeof(glm::vec4);
	normalArraySize = vertexNormals.size()*sizeof(glm::vec3);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexArraySize + colorArraySize + normalArraySize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexArraySize, (const GLvoid*)(&vertexPositions[0]));
	glBufferSubData(GL_ARRAY_BUFFER, vertexArraySize, colorArraySize, (const GLvoid*)(&vertexColors[0]));
	glBufferSubData(GL_ARRAY_BUFFER, vertexArraySize + colorArraySize, normalArraySize, (const GLvoid*)(&vertexNormals[0]));

	GLuint x = glGetAttribLocation(1, "vPosition");
	GLuint y = glGetAttribLocation(1, "vColor");
	GLuint z = glGetAttribLocation(1, "vNormal");

	glEnableVertexAttribArray(x);
	glVertexAttribPointer(x, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	
	glEnableVertexAttribArray(y);
	glVertexAttribPointer(y, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexArraySize));

	glEnableVertexAttribArray(z);
	glVertexAttribPointer(z, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexArraySize + colorArraySize) );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &vertexIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}