#version 450 core

layout (location=0) in vec2 vVertexPosition;
layout (location=1) in vec2 TextureCoord;

layout (location=0) out vec2 TxtCoord;

uniform mat3 uModelToNDC;
uniform vec2 TextureIndex;
uniform vec2 TextureDimensions;
uniform float ShakeTimer;
uniform bool ShakeScreen;

void main() 
{
		gl_Position = vec4(vec2(uModelToNDC * vec3(vVertexPosition, 1.f)), 0.0,
							1.0);
		
		float offsetX = TextureIndex.x * (1.0 / TextureDimensions.x);	
		float offsetY = TextureIndex.y * (1.0 / TextureDimensions.y);
		
		TxtCoord = vec2((TextureCoord.x / TextureDimensions.x) + offsetX, TextureCoord.y / TextureDimensions.y + offsetY);

		if(ShakeScreen)
		{
			float strength = 0.05f;
			gl_Position.x += cos(ShakeTimer * 10) * strength;        
			gl_Position.y += cos(ShakeTimer * 15) * strength;       
		}
}
