#ifndef BOX_HPP
#define BOX_HPP

#include "pch.h"
#include "glfw3.h"
#include "glm.hpp"
#include "GLM/glm/gtc/type_ptr.hpp"
#include "Graphics/Grid/bounds.h"

#define UPPER_BOUND 100

class Box {
public:
    std::vector<glm::vec3> offsets;
    std::vector<glm::vec3> sizes;

    void init() {
        vertices = {
            // position             x   y   z   i
             0.5f,  0.5f,  0.5f, // +   +   +   0
            -0.5f,  0.5f,  0.5f, // -   +   +   1
            -0.5f, -0.5f,  0.5f, // -   -   +   2
             0.5f, -0.5f,  0.5f, // +   -   +   3
             0.5f, 0.5f, -0.5f,  // +   +   -   4
            -0.5f, 0.5f, -0.5f,  // -   +   -   5
            -0.5f, -0.5f, -0.5f, // -   -   -   6
             0.5f, -0.5f, -0.5f  // +   -   -   7
        };

        indices = { // 12 lines
            // front face (+ve z)
            0, 1,
            1, 2,
            2, 3,
            3, 0,
            // back face (-ve z)
            4, 5,
            5, 6,
            6, 7,
            7, 4,
            // right face (+ve x)
            0, 4,
            3, 7,
            // left face (-ve x)
            1, 5,
            2, 6
        };

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

    void render(Shader shader, CameraComponent& camera)
    {
        GLint uniform_var_loc1 =shader.GetLocation("view");
        GLint uniform_var_loc2 =shader.GetLocation("projection");
        GLint uniform_var_loc3 = shader.GetLocation("model");
        GLint uniform_var_loc4 = shader.GetLocation("uModelToNDC");

        glm::mat4 _cameraprojMtx = glm::perspective(glm::radians(camera.fov), camera.aspect, camera.nearPlane, camera.farPlane);


        glm::mat4 mModelNDC;
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 rot{ 0,0,0 };
        glm::vec3 sclae{5,5,5};
        model = glm::translate(model, offsets[0]);
        model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, sizes[0]);

        mModelNDC = _cameraprojMtx * camera.viewMtx * model;
        glUniformMatrix4fv(uniform_var_loc4, 1, GL_FALSE, glm::value_ptr(mModelNDC));

        //glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(camera.viewMtx));
        //glUniformMatrix4fv(uniform_var_loc2, 1, GL_FALSE, glm::value_ptr(_cameraprojMtx));
        //glUniformMatrix4fv(uniform_var_loc3, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

        // update data
        int size = std::min(UPPER_BOUND, (int)offsets.size()); // if more than 100 instances, only render 100

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

    void addInstance(BoundingRegion br, glm::vec3 pos, glm::vec3 size) 
    {
        offsets.push_back(br.calculateCenter() * size + pos);
        sizes.push_back(br.calculateDimensions());
    }

    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &offsetVBO);
        glDeleteBuffers(1, &sizeVBO);
    }

private:
    unsigned int VAO, VBO, EBO;
    unsigned int offsetVBO, sizeVBO;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif