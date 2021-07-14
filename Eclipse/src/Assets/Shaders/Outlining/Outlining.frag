#version 450 core

layout (location=0) out vec4 fFragClr;

uniform vec4 uColor;

void main () 
{
    fFragClr = vec4(0.04, 0.28, 0.26, 1.0);
}
