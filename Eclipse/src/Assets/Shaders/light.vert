#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec2 TextureCoord;

layout (location=0) out vec2 TxtCoord;

uniform mat4 uModelToNDC;
uniform vec2 TextureIndex;
uniform vec2 TextureDimensions;

void main() 
{
		gl_Position =  uModelToNDC * vec4(vVertexPosition, 1.0f);
		
		float offsetX = TextureIndex.x * (1.0 / TextureDimensions.x);	
		float offsetY = TextureIndex.y * (1.0 / TextureDimensions.y);
		
		TxtCoord = vec2((TextureCoord.x / TextureDimensions.x) + offsetX, TextureCoord.y / TextureDimensions.y + offsetY);
}
