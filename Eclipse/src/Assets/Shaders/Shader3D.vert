#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec2 TextureCoord;
layout (location=2) in vec3 normal_from_vtxbuffer; 

layout (location=0) out vec2 TxtCoord;

uniform mat4 uModelToNDC;
uniform mat4 model;
uniform vec2 TextureIndex;
uniform vec2 TextureDimensions;
uniform bool TEST;

out vec3 crntPos;
out vec2 TexCoord;
out vec3 normal_from_vtxShader;

void main() 
{
        vec2 over;

		gl_Position =  uModelToNDC * vec4(vVertexPosition, 1.0f);

		crntPos = vec3(model * vec4(vVertexPosition, 1.0));

		normal_from_vtxShader = mat3(transpose(inverse(model))) * normal_from_vtxbuffer;  

		if(TEST)
		{
			float offsetX = TextureIndex.x * (1.0 / TextureDimensions.x);	
			float offsetY = TextureIndex.y * (1.0 / TextureDimensions.y);
		
			TxtCoord = vec2((TextureCoord.x / TextureDimensions.x) + offsetX, TextureCoord.y / TextureDimensions.y + offsetY);
		}

		TxtCoord = TextureCoord;
}
