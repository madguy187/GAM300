#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec2 TextureCoord;
layout (location=2) in vec3 normal_from_vtxbuffer; 

uniform mat4 uModelToNDC;

void main() 
{
		gl_Position =  uModelToNDC * vec4(vVertexPosition, 1.0f);
}
