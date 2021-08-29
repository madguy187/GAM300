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


        //glGenVertexArrays(1, &VAO);
        //glBindVertexArray(VAO);

        //glGenBuffers(1, &VBO);
        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        //glGenBuffers(1, &EBO);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        //glEnableVertexAttribArray(0);

        //glBindVertexArray(0);

                // generate VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // generate vertices VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // generate positions VBO
        glGenBuffers(1, &offsetVBO);
        glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
        glBufferData(GL_ARRAY_BUFFER, UPPER_BOUND * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // generate size VBO
        glGenBuffers(1, &sizeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
        glBufferData(GL_ARRAY_BUFFER, UPPER_BOUND * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // generate EBO
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set attribute pointers

        // vertex positions
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // offsets
        glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        // sizes
        glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glVertexAttribDivisor(1, 1); // reset _1st_ attribute every _1_ instance
        glVertexAttribDivisor(2, 1); // reset _2nd_ attribute every _1_ instance

        glBindVertexArray(0);
    }

    void AABB_::Render(Shader shader, CameraComponent& camera)
    {
        //GLint uniform_var_loc1 = shader.GetLocation("view");
        //GLint uniform_var_loc2 = shader.GetLocation("projection");
        //GLint uniform_var_loc3 = shader.GetLocation("model");
        //GLint uniform_var_loc4 = shader.GetLocation("uModelToNDC");

        //glm::mat4 _cameraprojMtx = glm::perspective(glm::radians(camera.fov), camera.aspect, camera.nearPlane, camera.farPlane);
        //glm::mat4 mModelNDC;
        //glm::mat4 model = glm::mat4(1.0f);
        //glm::vec3 rot{ 0,0,0 };

        //model = glm::translate(model, Position[0]);
        //model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        //model = glm::scale(model, AABB_Size[0]);

        //mModelNDC = _cameraprojMtx * camera.viewMtx * model;
        //glUniformMatrix4fv(uniform_var_loc4, 1, GL_FALSE, glm::value_ptr(mModelNDC));

        //// if more than 100 instances, only render 100
        //int size = min(UPPER_BOUND, (int)AABB_Size.size());

        //glEnable(GL_BLEND);
        //glEnable(GL_DEPTH_TEST);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glDisable(GL_CULL_FACE);
        //glEnable(GL_LINE_SMOOTH);

        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //// render instanced data
        //glBindVertexArray(VAO);
        //glDrawElementsInstanced(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0, size);
        //glBindVertexArray(0);

        GLint uniform_var_loc1 = shader.GetLocation("model");
        glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

        // update data
        int size = min(UPPER_BOUND, (int)offsets.size()); // if more than 100 instances, only render 100

        // update data
        if (size != 0) {
            // if instances exist

            glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3 * sizeof(float), &offsets[0]);

            glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3 * sizeof(float), &sizes[0]);

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
        glDrawElementsInstanced(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0, size);
        glBindVertexArray(0);
    }


    void AABB_::AddInstance(BoundingRegion br, glm::vec3 pos , glm::vec3 size)
    {
        //glm::vec3 positions = br.calculateCenter() + pos;
        //glm::vec3 dim = br.calculateDimensions();

        //Position.push_back(positions);
        //AABB_Size.push_back(dim);
        offsets.push_back(br.calculateCenter() * size + pos);
        //sizes.push_back(br.calculateDimensions() * size);
        sizes.push_back(size);
    }

    void AABB_::Cleanup()
    {
        //glDeleteVertexArrays(1, &VAO);
        //glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &offsetVBO);
        glDeleteBuffers(1, &sizeVBO);
    }

    void AABB_::Reset()
    {
        offsets.clear();
        sizes.clear();
    }

    //void AABB_::DrawAll(unsigned int FramebufferID)
    //{
    //    if (DrawAABBS)
    //    {
    //        if (Position.size() > 0)
    //        {
    //            glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID);
    //            CameraComponent camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
    //            auto shdrpgm = Graphics::shaderpgms.find("AABB");
    //            shdrpgm->second.Use();
    //            Render(shdrpgm->second, camera);
    //            shdrpgm->second.UnUse();
    //        }
    //    }
    //}
}
