#include "AABB.h"

AABB::AABB(){

}

AABB::AABB(float mX, float mY, float mZ, float mnX, float mnY, float mnZ){
	maxX = mX;
	maxY = mY;
	maxZ = mZ;

	minX = mnX;
	minY = mnY;
	minZ = mnZ;

	userData = nullptr;

	generateVertices();
	generateObjectBuffer();
}

void AABB::Draw(){
	/*
	glBindVertexArray(VAO);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	//glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, vertexIndices);

	glBindVertexArray(0);
	*/
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);

	for(int i=0; i<vertexPositions.size(); i++)
	{
		glVertex3f(vertexPositions[i].x, vertexPositions[i].y, vertexPositions[i].z);
	}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
}

void AABB::generateObjectBuffer()
{
	glGenVertexArrays( 1, &VAO);
	glBindVertexArray( VAO );

	numIndices = 48;
	vertexArraySize = vertexPositions.size()*sizeof(glm::vec3);
	colorArraySize = vertexColors.size()*sizeof(glm::vec4);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexArraySize + colorArraySize, NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, vertexArraySize, (const GLvoid*)(&vertexPositions[0]) );
	glBufferSubData( GL_ARRAY_BUFFER, vertexArraySize, colorArraySize, (const GLvoid*)(&vertexColors[0]));

	GLuint x = glGetAttribLocation(1, "vPosition");
	GLuint y = glGetAttribLocation(1, "vColor");

	glEnableVertexAttribArray( x ); //SimpleShader attrib at position 0 = "vPosition"
	glVertexAttribPointer( x, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	
	glEnableVertexAttribArray(y ); //SimpleShader attrib at position 1 = "vColor"
	glVertexAttribPointer( y, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexArraySize));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &vertexIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void AABB::generateVertices()
{
	
	//Colours
	glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 blue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 yellow = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 purple = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 cyan = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 orange = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
	glm::vec4 white = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

	//FrontFace
	vertexPositions.push_back(glm::vec3(minX, maxY, maxZ));		vertexColors.push_back(white);		//0
	vertexPositions.push_back(glm::vec3(maxX, maxY, maxZ));		vertexColors.push_back(white);		//1
	vertexPositions.push_back(glm::vec3(maxX, minY, maxZ));		vertexColors.push_back(white);		//2
	vertexPositions.push_back(glm::vec3(minX, minY, maxZ));		vertexColors.push_back(white);		//3

	//RightFace
	vertexPositions.push_back(glm::vec3(maxX, maxY, maxZ));		vertexColors.push_back(white);		//4
	vertexPositions.push_back(glm::vec3(maxX, maxY, minZ));		vertexColors.push_back(white);		//5
	vertexPositions.push_back(glm::vec3(maxX, minY, minZ));		vertexColors.push_back(white);		//6
	vertexPositions.push_back(glm::vec3(maxX, minY, maxZ));		vertexColors.push_back(white);		//7

	//BackFace
	vertexPositions.push_back(glm::vec3(maxX, maxY, minZ));		vertexColors.push_back(white);		//8
	vertexPositions.push_back(glm::vec3(minX, maxY, minZ));		vertexColors.push_back(white);		//9
	vertexPositions.push_back(glm::vec3(minX, minY, minZ));		vertexColors.push_back(white);		//10
	vertexPositions.push_back(glm::vec3(maxX, minY, minZ));		vertexColors.push_back(white);		//11

	//LeftFace
	vertexPositions.push_back(glm::vec3(minX, maxY, minZ));		vertexColors.push_back(white);		//12
	vertexPositions.push_back(glm::vec3(minX, maxY, maxZ));		vertexColors.push_back(white);		//13
	vertexPositions.push_back(glm::vec3(minX, minY, maxZ));		vertexColors.push_back(white);		//14
	vertexPositions.push_back(glm::vec3(minX, minY, minZ));		vertexColors.push_back(white);		//15

	//TopFace
	vertexPositions.push_back(glm::vec3(minX, maxY, minZ));		vertexColors.push_back(white);		//16
	vertexPositions.push_back(glm::vec3(maxX, maxY, minZ));		vertexColors.push_back(white);		//17
	vertexPositions.push_back(glm::vec3(maxX, maxY, maxZ));		vertexColors.push_back(white);		//18
	vertexPositions.push_back(glm::vec3(minX, maxY, maxZ));		vertexColors.push_back(white);		//19

	//BottomFace
	vertexPositions.push_back(glm::vec3(minX, minY, maxZ));		vertexColors.push_back(white);		//20
	vertexPositions.push_back(glm::vec3(maxX, minY, maxZ));		vertexColors.push_back(white);		//21
	vertexPositions.push_back(glm::vec3(maxX, minY, minZ));		vertexColors.push_back(white);		//22
	vertexPositions.push_back(glm::vec3(minX, minY, minZ));		vertexColors.push_back(white);		//23

	//CreateIndices --  For GL Lines, Just a test, this really aint efficient
	GLuint indices[] = { 
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		4, 5,
		5, 6,
		6, 7,
		7, 4,

		8, 9,
		9, 10,
		10, 11,
		11, 8,

		12, 13,
		13, 14,
		14, 15,
		15, 12,

		16, 17,
		17, 18,
		18, 19,
		19, 16,

		20, 21,
		21, 22,
		22, 23,
		23, 20

	};

	for(int i = 0; i<48; i++){
		vertexIndices[i] = indices[i];
	}
}

void AABB::Update(float mX, float mY, float mZ, float mnX, float mnY, float mnZ){

	maxX = mX;
	maxY = mY;
	maxZ = mZ;

	minX = mnX;
	minY = mnY;
	minZ = mnZ;

	vertexPositions[0] = glm::vec3(minX, maxY, maxZ);		//0
	vertexPositions[1] = glm::vec3(maxX, maxY, maxZ);		//1
	vertexPositions[2] = glm::vec3(maxX, minY, maxZ);		//2
	vertexPositions[3] = glm::vec3(minX, minY, maxZ);		//3

	//RightFace
	vertexPositions[4] = glm::vec3(maxX, maxY, maxZ);		//4
	vertexPositions[5] = glm::vec3(maxX, maxY, minZ);		//5
	vertexPositions[6] = glm::vec3(maxX, minY, minZ);		//6
	vertexPositions[7] = glm::vec3(maxX, minY, maxZ);		//7

	//BackFace
	vertexPositions[8] = glm::vec3(maxX, maxY, minZ);		//8
	vertexPositions[9] = glm::vec3(minX, maxY, minZ);		//9
	vertexPositions[10] = glm::vec3(minX, minY, minZ);		//10
	vertexPositions[11] = glm::vec3(maxX, minY, minZ);		//11

	//LeftFace
	vertexPositions[12] = glm::vec3(minX, maxY, minZ);		//12
	vertexPositions[13] = glm::vec3(minX, maxY, maxZ);		//13
	vertexPositions[14] = glm::vec3(minX, minY, maxZ);		//14
	vertexPositions[15] = glm::vec3(minX, minY, minZ);		//15

	//TopFace
	vertexPositions[16] = glm::vec3(minX, maxY, minZ);		//16
	vertexPositions[17] = glm::vec3(maxX, maxY, minZ);		//17
	vertexPositions[18] = glm::vec3(maxX, maxY, maxZ);		//18
	vertexPositions[19] = glm::vec3(minX, maxY, maxZ);		//19

	//BottomFace
	vertexPositions[20] = glm::vec3(minX, minY, maxZ);		//20
	vertexPositions[21] = glm::vec3(maxX, minY, maxZ);		//21
	vertexPositions[22] = glm::vec3(maxX, minY, minZ);		//22
	vertexPositions[23] = glm::vec3(minX, minY, minZ);		//23
}

bool AABB::checkCollision(AABB &b)
{
	if( maxX < b.minX || maxY < b.minY || maxZ < b.minZ ||
		minX > b.maxX || minY > b.maxY || minZ > b.maxZ) 
	{
		return false;
	}


	return true;
}

bool AABB::Contains(glm::vec3 pt)
{
	if( pt.x < minX || pt.y < minY || pt.z < minZ ||
		pt.x > maxX || pt.y > maxY || pt.z > maxZ)
	{
		return false;
	}

	return true;
}

bool AABB::Contains(AABB &b)
{
	if( b.maxX > maxX || b.maxY > maxY || b.maxZ > maxZ ||
		b.minX < minX || b.minY < minY || b.minZ < minZ)
	{
		return false;
	}
	return true;
}

bool AABB::TestRay(Ray3 ray)
{
	/*
	Ref: http://www.cs.utah.edu/~awilliam/box/box.pdf
	*/

	float txmin, txmax, tymin, tymax, tzmin, tzmax;
	float divx, divy, divz;

	divx = 1.0f / ray.dir.x;
	if(ray.dir.x >= 0)
	{
		txmin = (minX - ray.pos.x) *divx;
		txmax = (maxX - ray.pos.x) *divx;
	}
	else
	{
		txmin = (maxX - ray.pos.x) *divx;
		txmax = (minX - ray.pos.x) *divx;
	}

	divy = 1.0f / ray.dir.y;
	if(ray.dir.y >= 0)
	{
		tymin = (minY - ray.pos.y) *divy;
		tymax = (maxY - ray.pos.y) *divy;
	}
	else
	{
		tymin = (maxY - ray.pos.y) *divy;
		tymax = (minY - ray.pos.y) *divy;
	}

	if((txmin > tymax) || (tymin > txmax) )
	{
		return false;
	}

	if(tymin > txmin)
	{
		txmin = tymin;
	}
	if(tymax < txmax)
	{
		txmax = tymax;
	}

	divz = 1.0f / ray.dir.z;
	if(ray.dir.z >=0)
	{
		tzmin = (minZ - ray.pos.z) *divz;
		tzmax = (maxZ - ray.pos.z) *divz;
	}
	else
	{
		tzmin = (maxZ - ray.pos.z) *divz;
		tzmax = (minZ - ray.pos.z) *divz;
	}

	if((txmin > tzmin) || (tzmin > txmax))
	{
		return false;
	}
	/*
	if(tzmin > tzmin)
	{
		txmin = tzmin;
	}
	if(tzmax < txmax)
	{
		txmax = tzmax;
	}
	*/
	return true;


}

AABB AABB::Union(const AABB& aabb)
{
	float minXU = min(minX, aabb.minX);
	float minYU = min(minY, aabb.minY);
	float minZU = min(minZ, aabb.minZ);
	
	float maxXU = max(maxX, aabb.maxX);
	float maxYU = max(maxY, aabb.maxY);
	float maxZU = max(maxZ, aabb.maxZ);

	return AABB(maxXU, maxYU, maxZU, minXU, minYU, minZU);
}

void AABB::Union(AABB& u, const AABB& aabb){
	float minXU = min(minX, aabb.minX);
	float minYU = min(minY, aabb.minY);
	float minZU = min(minZ, aabb.minZ);
	
	float maxXU = max(maxX, aabb.maxX);
	float maxYU = max(maxY, aabb.maxY);
	float maxZU = max(maxZ, aabb.maxZ);

	u.Update(maxXU, maxYU, maxZU, minXU, minYU, minZU);
}

float AABB::Volume()
{
	float lenX = maxX - minX;
	float lenY = maxY - minY;
	float lenZ = maxZ - minZ;

	return lenX*lenY*lenZ;
}

float AABB::UnionVolume(const AABB& aabb){
	float minXU = min(minX, aabb.minX);
	float minYU = min(minY, aabb.minY);
	float minZU = min(minZ, aabb.minZ);
	
	float maxXU = max(maxX, aabb.maxX);
	float maxYU = max(maxY, aabb.maxY);
	float maxZU = max(maxZ, aabb.maxZ);

	float lenX = maxXU - minXU;
	float lenY = maxYU - minYU;
	float lenZ = maxZU - minZU;

	return lenX * lenY * lenZ;
}

void AABB::setBroadColliding(bool state)
{
	isBroadColliding = state;

	glm::vec4 color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	if(state)
	{
		color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	vertexColors[0] = color;
	vertexColors[1] = color;
	vertexColors[2] = color;
	vertexColors[3] = color;
		
	vertexColors[4] = color;
	vertexColors[5] = color;
	vertexColors[6] = color;
	vertexColors[7] = color;
	
	vertexColors[8] = color;
	vertexColors[9] = color;
	vertexColors[10] = color;
	vertexColors[11] = color;
		
	vertexColors[12] = color;
	vertexColors[13] = color;
	vertexColors[14] = color;
	vertexColors[15] = color;
		
	vertexColors[16] = color;
	vertexColors[17] = color;
	vertexColors[18] = color;
	vertexColors[19] = color;
		
	vertexColors[20] = color;
	vertexColors[21] = color;
	vertexColors[22] = color;
	vertexColors[23] = color;
	
}