#version 450 core

in vec4 uColor;
in vec2 TxtCoord;
in float vTexIndex;

out vec4 fFragClr;

uniform sampler2D uTex2d;

void main()
{
	fFragClr = uColor * texture( uTex2d, TxtCoord );
}