#version 460 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec2 TextureCoord;
layout (location=2) in vec3 normal_from_vtxbuffer; 

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 uModelToNDC;

void main()
{
    TexCoords = TextureCoord;
    WorldPos = vec3(model * vec4(vVertexPosition, 1.0));
    Normal = mat3(model) * normal_from_vtxbuffer;   

    gl_Position =  uModelToNDC * vec4(vVertexPosition, 1.0);
}