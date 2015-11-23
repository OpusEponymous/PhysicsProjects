#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 3) in vec4 vColor;


uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

out vec color;

void main()
{
	color = vColor;
    gl_Position = pMatrix *vMatrix * mMatrix * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
	
}
