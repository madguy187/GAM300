#version 450 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vVertexPosition;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 TextureCoord;
layout(location = 3) in float TexIndex;

uniform mat3 uModelToNDC;

out vec4 uColor;
out vec2 TxtCoord;
out float vTexIndex;

void main()
{
	uColor = color;
	TxtCoord = TextureCoord;
	vTexIndex = TexIndex;
	
	gl_Position = vec4(vec2(uModelToNDC * vec3(vVertexPosition, 1.f)), 0.0,
							1.0);
							
}

