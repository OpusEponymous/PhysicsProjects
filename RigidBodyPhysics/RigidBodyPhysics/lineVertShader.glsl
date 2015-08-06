#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 3) in vec3 vColor;


uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

out vec3 color;

void main()
{
	color = vColor;
    gl_Position = pMatrix *vMatrix * mMatrix * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
	
}
