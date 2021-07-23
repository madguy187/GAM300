#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aOffset;
layout (location = 2) in vec3 aSize;

uniform mat4 uModelToNDC;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
	gl_Position = uModelToNDC * vec4(aPos, 1.0);
}