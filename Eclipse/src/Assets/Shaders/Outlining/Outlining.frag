#version 450 core

layout (location=0) out vec4 fFragClr;

uniform vec3 HighlightColour;

void main () 
{
    fFragClr = vec4(HighlightColour, 1.0);
}
