#pragma once
#include "GLM/glm/gtc/quaternion.hpp"

namespace Eclipse
{
    class DEBUG_SPHERE
    {
    public:
        std::vector <Vertex> vertices;
        std::vector <unsigned int> indices;
        std::vector <Texture> textures;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int instanceVBO;
        unsigned int EBO;
        glm::mat4 CamMatrix;
        unsigned int instancing;

        DEBUG_SPHERE() {};
        void Init(std::vector <Vertex>& vertices,std::vector <unsigned int>& indices,unsigned int instancing = 1,std::vector <glm::mat4> instanceMatrix = {});
        void Draw(Shader& shader, CameraComponent& camera, glm::mat4 matrix = glm::mat4(1.0f), glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f), glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(5.0f, 5.0f, 5.0f));
    };
}
