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
        // Store VAO in public so it can be used in the Draw function
        unsigned int VAO;
        unsigned int VBO;
        unsigned int instanceVBO;
        unsigned int EBO;
        glm::mat4 CamMatrix;

        // Holds number of instances (if 1 the mesh will be rendered normally)
        unsigned int instancing;

        DEBUG_SPHERE() {};

        // Initializes the mesh
        DEBUG_SPHERE
        (
            std::vector <Vertex>& vertices,
            std::vector <unsigned int>& indices,
            unsigned int instancing = 1,
            std::vector <glm::mat4> instanceMatrix = {}
        );

        void Init
        (
            std::vector <Vertex>& vertices,
            std::vector <unsigned int>& indices,
            unsigned int instancing = 1,
            std::vector <glm::mat4> instanceMatrix = {}
        );

        // Draws the mesh
        void Draw
        (
            Shader& shader,
            CameraComponent& camera,
            glm::mat4 matrix = glm::mat4(1.0f),
            glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
        );
    };
}
