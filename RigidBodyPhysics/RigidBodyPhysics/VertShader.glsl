#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec4 vColor;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

out vec4 color;

void main()
{
	color = vColor;
	//color = vec4(1.0, 1.0, 1.0, 1.0);
    gl_Position = pMatrix *vMatrix * mMatrix * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
	
}