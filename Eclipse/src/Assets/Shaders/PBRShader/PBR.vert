#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 4) in ivec4 boneIds;
layout (location = 5) in vec4 weights;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool hasAnimation;
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    TexCoords = aTexCoords;

    if(hasAnimation)
    {
        vec4 totalPosition = vec4(0.0f);
        vec3 localNormal;

        for(int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        {
            if(boneIds[i] == -1)
            {
                continue;
            }
        
            if(boneIds[i] >= MAX_BONES)
            {
                totalPosition = vec4(aPos, 1.0f);
                break;
            }
        
            vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos, 1.0f);
            totalPosition += localPosition * weights[i];
            localNormal = mat3(finalBonesMatrices[boneIds[i]]) * aNormal;
        }

            mat4 viewModel = view * model;
            Normal = mat3(model) * aNormal;
            //Normal = mat3(model) * localNormal;
            gl_Position =  projection * viewModel * totalPosition;
    }
    else
    {
        WorldPos = vec3(model * vec4(aPos, 1.0));
        Normal = mat3(model) * aNormal;   
        gl_Position =  projection * view * vec4(WorldPos, 1.0);
    }
}