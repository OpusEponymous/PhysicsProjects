#include "Shader.h"

Shader::Shader()
{
	ID = glCreateProgram();
	if(ID == 0)
	{
		fprintf(stderr, "Error creating chader program\n");
		exit(1);
	}
}

GLuint Shader::getID()
{
	return ID;
}

string Shader::fileRead(string filename)
{
	ifstream File(filename);
	string str;
	char* line = new char[256];

	if(File.is_open())
	{
		while(File.getline(line, 256))
		{
			str += line;
			str += "\n";
		}
	}
	File.close();
	delete[] line;

	return str;
}

void Shader::Compile(string vsFile, string fsFile)
{
	string vS = fileRead(vsFile);
	const char* vShader = vS.c_str();

	string fS = fileRead(fsFile);
	const char* fShader = fS.c_str();

	addShader(ID, vShader, GL_VERTEX_SHADER);
	addShader(ID, fShader, GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = {0};

	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &Success);
	if(Success == 0)
	{
		glGetProgramInfoLog(ID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: %s \n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ID);

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
    glValidateProgram(ID);
	// check for program related errors using glGetProgramiv
    glGetProgramiv(ID, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ID, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }
	
	/*
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
    glUseProgram(ID);
	*/
}

void Shader::addShader(GLuint progID, const char* shaderText, GLenum shaderType)
{
	GLuint shaderObj = glCreateShader(shaderType);

	if (shaderObj == 0)
	{
		fprintf(stderr, "Error creating shader type %d \n", shaderType);
		exit(0);
	}

	//Bind shader GLSL code to shader object
	glShaderSource(shaderObj, 1, (const GLchar**)&shaderText, NULL);
	//Compile shader object
	glCompileShader(shaderObj);
	//Check for compilation errors
	GLint success;
	// check for shader related errors using glGetShaderiv
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
        exit(1);
    }

	//attach compiled shader to shader program
	glAttachShader(progID, shaderObj);

}

void Shader::Use()
{
	glUseProgram(ID);
}