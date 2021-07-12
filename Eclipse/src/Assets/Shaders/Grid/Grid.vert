#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec2 TextureCoord;
layout (location=2) in vec3 normal_from_vtxbuffer; 

layout(location = 3) out vec3 nearPoint;
layout(location = 4) out vec3 farPoint;

uniform mat4 uModelToNDC;
uniform mat4 viewMtx;
uniform mat4 projMtx;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) 
{
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() 
{
    nearPoint = UnprojectPoint(vVertexPosition.x, vVertexPosition.y, 0.0,viewMtx, projMtx);
    farPoint = UnprojectPoint(vVertexPosition.x, vVertexPosition.y, 1.0, viewMtx, projMtx); 
    gl_Position = vec4(vVertexPosition, 1.0);
}
