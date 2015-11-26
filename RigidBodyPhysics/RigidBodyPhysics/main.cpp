#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include "Camera.h"
#include "Shader.h"
#include "Plane.h"
#include "NSquared.h"
#include "AABBTree.h"

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define NUM_RIGIDBODIES 8
#define WIDTH 1280
#define HEIGHT 720

using namespace std;

GLuint VBO;
float prevTime = 0.0;

glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projMatrix;

const float dt = 0.01f;
float t = 0.1f;
float timeAccumulator = 0.0f;
float currentTime = 0.0f;

int drawLayer = 3;

Camera cam;
Shader *myShader;
Shader *lineShader;
RigidBody rBody;

vector<RigidBody> rigidBodies;
NSquared nSqBroadPhase;
AABBTree aabbTree;

void init();
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int Key, int x, int y);
void mouseMotion(int x, int y);

#pragma region VBO_FUNCTIONS

void linkCurrentBuffertoShader(GLuint shaderProgramID)
{
	GLuint numVertices = 6;
	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	GLuint texID = glGetAttribLocation(shaderProgramID, "vTexC");

	// Have to enable this
	glEnableVertexAttribArray(positionID);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
    glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Similarly, for the color data.
	glEnableVertexAttribArray(texID);
	glVertexAttribPointer(texID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices*sizeof(glm::vec3)));
}
#pragma endregion VBO_FUNCTIONS

void display()
{
	cam.onRender();
	cam.calcMatrix();	
	viewMatrix = cam.vMat;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float newTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	float timeDelta = newTime - currentTime;

	currentTime = newTime;

	//update
	timeAccumulator += timeDelta;
	
	while(timeAccumulator >=dt)
	{
		for(int i=0; i<NUM_RIGIDBODIES; i++){
			rigidBodies[i].Update(t, dt);
			//rigidBodies[i].applyTransform(rigidBodies[i].currentState);
			rigidBodies[i].calcBoundingBox();
			rigidBodies[i].setBroadColliding(false);
			rigidBodies[i].setNarrowColliding(false);			
		}

		aabbTree.Update();
		//vector<pair<RigidBody *, RigidBody *>> CollidingPairs = nSqBroadPhase.ComputePairs();
		vector<pair<RigidBody *, RigidBody *>> CollidingPairs = aabbTree.ComputePairs();


		if(CollidingPairs.size()>0)
		{
			for (int i=0; i<CollidingPairs.size(); i++)
			{
				CollidingPairs[i].first->setBroadColliding(true);
				CollidingPairs[i].second->setBroadColliding(true);
			}
		}

		

		//rBody.Update(t, dt);
		timeAccumulator -= dt;
		t += dt;
	}

	myShader->Use();

	GLuint vMatID = glGetUniformLocation(myShader->getID(), "vMatrix");
	glUniformMatrix4fv(vMatID, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	GLuint pMatID = glGetUniformLocation(myShader->getID(), "pMatrix");
	glUniformMatrix4fv(pMatID, 1, GL_FALSE, glm::value_ptr(projMatrix));

	for(int i=0; i<NUM_RIGIDBODIES; i++){
		GLuint mMatID = glGetUniformLocation(myShader->getID(), "mMatrix");
		glUniformMatrix4fv(mMatID, 1, GL_FALSE, glm::value_ptr(rigidBodies[i].currentState.modelMatrix));

		rigidBodies[i].Draw();
	}

	lineShader->Use();
	glm::mat4 identity = glm::mat4(1.0f);

	GLuint mMatID = glGetUniformLocation(lineShader->getID(), "mMatrix");
	glUniformMatrix4fv(mMatID, 1, GL_FALSE, glm::value_ptr(identity));

	vMatID = glGetUniformLocation(lineShader->getID(), "vMatrix");
	glUniformMatrix4fv(vMatID, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	pMatID = glGetUniformLocation(lineShader->getID(), "pMatrix");
	glUniformMatrix4fv(pMatID, 1, GL_FALSE, glm::value_ptr(projMatrix));

	GLuint pColID = glGetUniformLocation(lineShader->getID(), "pColor");
	glm::vec4 white = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	glUniform4fv(pColID, 1, glm::value_ptr(white));
	aabbTree.Draw(drawLayer);

	/*
	for(int i=0; i<NUM_RIGIDBODIES; i++){

		if(rigidBodies[i].boundingBox.isBroadColliding)
		{
			glUniform4fv(pColID, 1, glm::value_ptr(red));
		}
		else
		{
			glUniform4fv(pColID, 1, glm::value_ptr(white));
		}

		rigidBodies[i].boundingBox.Draw();
	}
	*/
    glutSwapBuffers();
}

int main(int argc, char** argv){

	// Set up the window
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Fizzicks Yo");
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(display);
	 // A call to glewInit() must be done after glut is initialized!
    GLenum res = glewInit();
	// Check for any errors
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }

	// Set up your objects and shaders
	init();

	// Begin infinite event loop
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( specialKeyboard );
	glutPassiveMotionFunc( mouseMotion );
	glutMainLoop();
	return 0;
}



void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	modelMatrix = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);

	//cam = Camera(40.0f, 60.0f, 0.0f, glm::vec3(0,0,0), glm::vec3(0,1,0));
	cam = Camera((int)WIDTH, (int)HEIGHT, glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0001f, 1.0f), glm::vec3(0.0f,1.0f,0.0f));
	cam.calcMatrix();

	viewMatrix = cam.vMat;

	projMatrix = glm::perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);


	myShader = new Shader();
	myShader->Compile("VertShader.glsl", "FragShader.glsl");

	lineShader = new Shader();
	lineShader->Compile("lineVertShader.glsl", "lineFragShader.glsl");

	myShader->Use();


	for(int i=0; i<NUM_RIGIDBODIES; i++){
		rBody = RigidBody(2.0f, 1.0f, i);
		rBody.attachSpring(glm::vec3(0.0f,5.0f,0.0f));
		rBody.currentState.position = glm::vec3(-15.0f + (8.0f*i), 0.0f, -10.0f + (8.0f*i));
		rBody.currentState.recalculate();
		rBody.calcBoundingBox();
		rigidBodies.push_back(rBody);
	}
	/*
	for(int i=0; i<NUM_RIGIDBODIES; i++){
		nSqBroadPhase.Add(&rigidBodies[i]);
	}
	*/
	
	for(int i=0; i<NUM_RIGIDBODIES; i++){
		aabbTree.Add(&rigidBodies[i]);
	}
	
	GLuint mMatID = glGetUniformLocation(myShader->getID(), "mMatrix");
	glUniformMatrix4fv(mMatID, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	GLuint vMatID = glGetUniformLocation(myShader->getID(), "vMatrix");
	glUniformMatrix4fv(vMatID, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	GLuint pMatID = glGetUniformLocation(myShader->getID(), "pMatrix");
	glUniformMatrix4fv(pMatID, 1, GL_FALSE, glm::value_ptr(projMatrix));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

#pragma region IO_Functions

void keyboard(unsigned char key, int x, int y){
	if(key == 'W' || key == 'w'){
		for(int i=0; i< NUM_RIGIDBODIES; i++){
			rigidBodies[i].springPos = glm::vec3(5.0f, 5.0f, 0.0f);
		}
	}

	if(key == 'A' || key == 'a'){
		for(int i=0; i< NUM_RIGIDBODIES; i++){
			rigidBodies[i].springPos = glm::vec3(-5.0f, -5.0f, 0.0f);
		}	
	}

	if(key == 'S' || key == 's'){
		for(int i=0; i< NUM_RIGIDBODIES; i++){
			rigidBodies[i].springPos = glm::vec3(5.0f, -5.0f, 0.0f);
		}
	}

	if(key == 'Q' || key == 'q'){
		for(int i=0; i< NUM_RIGIDBODIES; i++){
			rigidBodies[i].springPos = glm::vec3(-5.0f, 5.0f, 0.0f);
		}
	}

	if(key == '+')
	{
		drawLayer++;
	}

	if(key == '-')
	{
		drawLayer--;
	}
}

void specialKeyboard(int Key, int x, int y){
	//cam.handleKeyboardOrbital(Key);
	cam.handleKeyboardFPS(Key);

	cam.calcMatrix();
	
	viewMatrix = cam.vMat;
	
	GLuint vMatID = glGetUniformLocation(myShader->getID(), "vMatrix");
	glUniformMatrix4fv(vMatID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void mouseMotion(int x, int y)
{
	cam.handleMouse(x, y);
	cam.calcMatrix();
	viewMatrix = cam.vMat;
}
#pragma endregion IO_Functions