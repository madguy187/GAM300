#version 450 core

layout (location=0) in vec2 TxtCoord;

layout (location=0) out vec4 fFragClr;

uniform vec4 uColor;

void main () 
{
	float distance = length(TxtCoord);
	fFragClr = vec4(uColor.rgb, uColor.a * (pow(0.01, distance) - 0.01));
}