#version 450 core

layout(location = 3) in vec3 nearPoint;
layout(location = 4) in vec3 farPoint; 

layout(location=0) out vec4 fFragClr;

uniform vec4 lightColor;
uniform float QuadScale;

vec4 grid(vec3 fragPos3D, float scale) 
{
    vec2 coord = fragPos3D.xz / scale; // use the scale variable to set the distance between the lines
    vec2 derivative = fwidth(coord);

    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0, 0, 0, 1.0 - min(line, 1.0));

    // z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
    {
            color.z = 1.0;
    }

    // x axis
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
    {
            color.x = 1.0;
    }

    return color;
}

void main () 
{
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
    fFragClr = grid(fragPos3D, QuadScale) * float(t > 0);
}
