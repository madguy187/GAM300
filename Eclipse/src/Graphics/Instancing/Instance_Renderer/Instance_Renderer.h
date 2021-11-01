#pragma once
#include "GLM/glm/gtc/quaternion.hpp"

namespace Eclipse
{
    class Instance_Renderer
    {
    public:
        std::vector <Vertex> Vertices;
        std::vector <unsigned int> Indices;
        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int InstanceVBO = 0;
        unsigned int EBO = 0;
        glm::mat4 CamMatrix{ 1.0f };
        unsigned int Instancing = 0;
        bool DrawModel = true;

        Instance_Renderer() {};
        void Init(std::vector <Vertex>& vertices, std::vector <unsigned int>& indices, unsigned int instancing = 1, std::vector <glm::mat4> instanceMatrix = {});
        void Draw(Shader& shader, CameraComponent& camera);
    };
}
