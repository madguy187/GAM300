#version 450 core

layout (location=0) out vec4 fFragClr;

uniform vec4 uColor;

void main () 
{
    fFragClr = vec4(uColor);
}
