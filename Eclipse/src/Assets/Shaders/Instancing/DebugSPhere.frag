#version 450 core

layout (location=0) out vec4 fFragClr;
uniform vec3 Colour;

void main () 
{
    fFragClr = vec4(Colour, 1.0);
}