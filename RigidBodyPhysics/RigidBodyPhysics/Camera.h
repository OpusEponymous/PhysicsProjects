#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
	glm::mat4 vMat;
	Camera();
	Camera(float r, float theta, float phi, glm::vec3 cTarget, glm::vec3 cUP);
	Camera(int WindowWidth, int WindowHeight, glm::vec3 Pos, glm::vec3 Target, glm::vec3 Up);

	void calcMatrix();
	void calcMatrix(glm::vec3 targetPt);
	bool handleKeyboardOrbital(int key);
	bool handleKeyboardFPS(int key);
	void handleMouse(int x, int y);
	void onRender();
	

private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;

	float R;
	float Theta;
	float Phi;

	int windowWidth;
	int windowHeight;

	bool onUpperEdge;
	bool onLowerEdge;
	bool onLeftEdge;
	bool onRightEdge;

	bool isOrbital;

	glm::vec2 mousePos;

	void Init();
	void Update();
};

#endif