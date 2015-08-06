#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "Cube.h"
//#include "Derivative.h"
//#include "State.h"

#ifndef COMMON_H
#define COMMON_H

using namespace std;

static glm::vec3 operator*(glm::mat4 mat, glm::vec3 vec){
	return glm::vec3(mat * glm::vec4(vec, 1));
}

#endif