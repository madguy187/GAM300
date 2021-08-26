#include "pch.h"
#include "Graphics/Grid/Box.h"

namespace Eclipse
{
    AABB::AABB() :
        VAO{ 1 },
        VBO{ 1 },
        EBO{ 1 }
    {

    }

    void AABB::Init()
    {
        vertices =
        {
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f
        };

        indices =
        {
            // front face 
            0, 1,
            1, 2,
            2, 3,
            3, 0,

            // back face
            4, 5,
            5, 6,
            6, 7,
            7, 4,

            // right face 
            0, 4,
            3, 7,

            // left face 
            1, 5,
            2, 6
        };


        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    void AABB::Render(Shader shader, CameraComponent& camera)
    {
        GLint uniform_var_loc1 = shader.GetLocation("view");
        GLint uniform_var_loc2 = shader.GetLocation("projection");
        GLint uniform_var_loc3 = shader.GetLocation("model");
        GLint uniform_var_loc4 = shader.GetLocation("uModelToNDC");

        glm::mat4 _cameraprojMtx = glm::perspective(glm::radians(camera.fov), camera.aspect, camera.nearPlane, camera.farPlane);
        glm::mat4 mModelNDC;
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 rot{ 0,0,0 };

        model = glm::translate(model, Position[0]);
        model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, AABB_Size[0]);

        mModelNDC = _cameraprojMtx * camera.viewMtx * model;
        glUniformMatrix4fv(uniform_var_loc4, 1, GL_FALSE, glm::value_ptr(mModelNDC));

        // if more than 100 instances, only render 100
        int size = min(UPPER_BOUND, (int)AABB_Size.size());

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glEnable(GL_LINE_SMOOTH);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // render instanced data
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0, size);
        glBindVertexArray(0);
    }

    void AABB::AddInstance(BoundingRegion br, glm::vec3 pos)
    {
        glm::vec3 positions = br.calculateCenter() + pos;
        glm::vec3 dim = br.calculateDimensions();

        Position.push_back(positions);
        AABB_Size.push_back(dim);
    }

    void AABB::Cleanup()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void AABB::Reset()
    {
        Position.clear();
        AABB_Size.clear();
    }

    void AABB::DrawAll(unsigned int FramebufferID)
    {
        if (DrawAABBS)
        {
            if (Position.size() > 0)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID);
                CameraComponent camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
                auto shdrpgm = Graphics::shaderpgms.find("AABB");
                shdrpgm->second.Use();
                Render(shdrpgm->second, camera);
                shdrpgm->second.UnUse();
            }
        }
    }
}
