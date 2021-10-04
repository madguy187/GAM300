#include "pch.h"
#include "Graphics/Grid/Box.h"

namespace Eclipse
{
    AABB_::AABB_() :
        VAO{ 1 },
        VBO{ 1 },
        EBO{ 1 }
    {

    }

    void AABB_::Init()
    {
        Vertices =
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

        Indices =
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
        // generate VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // generate vertices VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // generate positions VBO
        glGenBuffers(1, &OffsetVBO);
        glBindBuffer(GL_ARRAY_BUFFER, OffsetVBO);
        glBufferData(GL_ARRAY_BUFFER, UPPER_BOUND * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // generate size VBO
        glGenBuffers(1, &SizeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, SizeVBO);
        glBufferData(GL_ARRAY_BUFFER, UPPER_BOUND * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // generate EBO
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

        // set attribute pointers

        // vertex positions
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // offsets
        glBindBuffer(GL_ARRAY_BUFFER, OffsetVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        // sizes
        glBindBuffer(GL_ARRAY_BUFFER, SizeVBO);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);

        glBindVertexArray(0);
    }

    void AABB_::Render(Shader& shader)
    {
        GLint uniform_var_loc1 = shader.GetLocation("model");
        glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

        // update data
        int size = min(UPPER_BOUND, (int)Offsets.size());

        // update data
        if (size != 0)
        {
            // if instances exist

            glBindBuffer(GL_ARRAY_BUFFER, OffsetVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3 * sizeof(float), &Offsets[0]);

            glBindBuffer(GL_ARRAY_BUFFER, SizeVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3 * sizeof(float), &Sizes[0]);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glEnable(GL_LINE_SMOOTH);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // render instanced data
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_LINES, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, 0, size);
        glBindVertexArray(0);
    }


    void AABB_::AddInstance(BoundingRegion& br)
    {
        Offsets.push_back(br.calculateCenter() * br.Scale + br.Position);
        Sizes.push_back(br.Scale);
    }

    void AABB_::Cleanup()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &OffsetVBO);
        glDeleteBuffers(1, &SizeVBO);
    }

    void AABB_::Reset()
    {
        Offsets.clear();
        Sizes.clear();

        engine->GridManager->InsertAsDebugBox();
    }

    bool AABB_::CheckToRender()
    {
        if (Offsets.size() > 0)
        {
            return true;
        }

        return false;
    }

    bool AABB_::ShouldRender()
    {
        return DrawAABBS;
    }

    void AABB_::DrawAll(unsigned int FramebufferID)
    {
        if (DrawAABBS)
        {
            if (Offsets.size() > 0)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID);

                CameraComponent camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
                auto shdrpgm = Graphics::shaderpgms["AABB"];
                shdrpgm.Use();

                GLint uniform_var_loc1 = shdrpgm.GetLocation("view");
                GLint uniform_var_loc2 = shdrpgm.GetLocation("projection");
                glm::mat4 _cameraprojMtx = glm::perspective(glm::radians(camera.fov), camera.aspect, camera.nearPlane, camera.farPlane);
                glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(camera.viewMtx));
                glUniformMatrix4fv(uniform_var_loc2, 1, GL_FALSE, glm::value_ptr(_cameraprojMtx));

                engine->GraphicsManager.AllAABBs.Render(shdrpgm);
                shdrpgm.UnUse();
            }
        }
    }
}
