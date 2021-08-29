#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 uModelToNDC;

void main() 
{
	gl_Position = uModelToNDC * vec4(aPos, 1.0);
}