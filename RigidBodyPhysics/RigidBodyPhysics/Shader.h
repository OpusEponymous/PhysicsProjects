#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include<iostream>
#include<fstream>

#ifndef SHADER_H
#define SHADER_H

using namespace std;

class Shader
{
public:
	Shader();
	GLuint getID();

	void Compile(string, string);
	void Use();

private:
	GLuint ID;
	void addShader(GLuint, const char*, GLenum);
	string fileRead(string);
};

#endif