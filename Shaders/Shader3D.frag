#version 450 core

layout (location=0) in vec2 TxtCoord;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uTex2d;

uniform vec4 uColor;
uniform bool uTextureCheck;
uniform float LightTimer;

void main () 
{
	if(!uTextureCheck)
	{
		fFragClr = vec4(uColor);
	}
	else
	{
		fFragClr = vec4(LightTimer * 1.0f, LightTimer * 1.0f, LightTimer * 1.0f, uColor.a) * texture(uTex2d, TxtCoord);
			
		if (fFragClr.a == 0.0) 
		{
			discard;
		}
	
	}
}
