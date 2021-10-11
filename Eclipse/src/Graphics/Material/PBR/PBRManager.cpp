#include "pch.h"
#include "PBRManager.h"

namespace Eclipse
{
    void PBRManager::SetShader()
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));

        auto& shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        shdrpgm.setInt("albedoMap", 0);
        shdrpgm.setInt("normalMap", 1);
        shdrpgm.setInt("metallicMap", 2);
        shdrpgm.setInt("roughnessMap", 3);
        shdrpgm.setInt("aoMap", 4);

        GLint uniform_var_loc1 = shdrpgm.GetLocation("albedo");
        GLint uniform_var_loc2 = shdrpgm.GetLocation("ao");
        GLuint cameraPos = shdrpgm.GetLocation("camPos");
        GLuint view = shdrpgm.GetLocation("view");
        GLint projection = shdrpgm.GetLocation("projection");

        GLCall(glUniform3f(uniform_var_loc1, 0.5f, 0.0f, 0.0f));
        GLCall(glUniform1f(uniform_var_loc2, 1.0f));
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
        GLCall(glUniform3f(cameraPos, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ()));
        shdrpgm.UnUse();


        albedo = loadTexture("src/Assets/PBR/albedo.png");
        normal = loadTexture("src/Assets/PBR/normal.png");
        metallic = loadTexture("src/Assets/PBR/metallic.png");
        roughness = loadTexture("src/Assets/PBR/roughness.png");
        ao = loadTexture("src/Assets/PBR/ao.png");
    }

    void PBRManager::RenderSphere()
    {
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (sphereVAO == 0)
        {
            glGenVertexArrays(1, &sphereVAO);

            unsigned int vbo, ebo;
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);

            std::vector<glm::vec3> positions;
            std::vector<glm::vec2> uv;
            std::vector<glm::vec3> normals;
            std::vector<unsigned int> indices;

            const unsigned int X_SEGMENTS = 64;
            const unsigned int Y_SEGMENTS = 64;
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
                {
                    float xSegment = (float)x / (float)X_SEGMENTS;
                    float ySegment = (float)y / (float)Y_SEGMENTS;
                    float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                    float yPos = std::cos(ySegment * PI);
                    float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                    positions.push_back(glm::vec3(xPos, yPos, zPos));
                    uv.push_back(glm::vec2(xSegment, ySegment));
                    normals.push_back(glm::vec3(xPos, yPos, zPos));
                }
            }

            bool oddRow = false;
            for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
            {
                if (!oddRow) // even rows: y == 0, y == 2; and so on
                {
                    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                    {
                        indices.push_back(y * (X_SEGMENTS + 1) + x);
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    }
                }
                else
                {
                    for (int x = X_SEGMENTS; x >= 0; --x)
                    {
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                        indices.push_back(y * (X_SEGMENTS + 1) + x);
                    }
                }
                oddRow = !oddRow;
            }
            indexCount = indices.size();

            std::vector<float> data;
            for (unsigned int i = 0; i < positions.size(); ++i)
            {
                data.push_back(positions[i].x);
                data.push_back(positions[i].y);
                data.push_back(positions[i].z);
                if (normals.size() > 0)
                {
                    data.push_back(normals[i].x);
                    data.push_back(normals[i].y);
                    data.push_back(normals[i].z);
                }
                if (uv.size() > 0)
                {
                    data.push_back(uv[i].x);
                    data.push_back(uv[i].y);
                }
            }
            glBindVertexArray(sphereVAO);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
            unsigned int stride = (3 + 2 + 3) * sizeof(float);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        }

        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
    }

    void PBRManager::UpdateLoop()
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));


        int nrRows = 7;
        int nrColumns = 7;
        float spacing = 2.5;

        auto& shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        glm::mat4 model = glm::mat4(1.0f);
        for (int row = 0; row < nrRows; ++row)
        {
            GLuint metallic = shdrpgm.GetLocation("metallic");
            GLCall(glUniform1f(metallic, (float)row / (float)nrRows));

            for (int col = 0; col < nrColumns; ++col)
            {
                // we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                GLuint roughness = shdrpgm.GetLocation("roughness");
                GLint uModelToNDC_ = shdrpgm.GetLocation("uModelToNDC");
                GLint model_ = shdrpgm.GetLocation("model");
                GLuint view = shdrpgm.GetLocation("view");
                GLint projection = shdrpgm.GetLocation("projection");

                glm::mat4 mModelNDC;
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((col - (nrColumns / 2)) * spacing, (row - (nrRows / 2)) * spacing, 0.0f));
                model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, { 1,1,1 });
                mModelNDC = _camera.projMtx * _camera.viewMtx * model;
                GLCall(glUniform1f(roughness, glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f)));
                glUniformMatrix4fv(uModelToNDC_, 1, GL_FALSE, glm::value_ptr(mModelNDC));
                glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
                glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
                RenderSphere();
            }
        }

        // lights
// ------
        glm::vec3 lightPositions[] = {
              glm::vec3(-10.0f,  10.0f, 10.0f),
              glm::vec3(10.0f,  10.0f, 10.0f),
              glm::vec3(-10.0f, -10.0f, 10.0f),
              glm::vec3(10.0f, -10.0f, 10.0f),
        };
        glm::vec3 lightColors[] = {
             glm::vec3(300.0f, 300.0f, 300.0f),
             glm::vec3(300.0f, 300.0f, 300.0f),
             glm::vec3(300.0f, 300.0f, 300.0f),
             glm::vec3(300.0f, 300.0f, 300.0f)
        };

        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
        {
            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions[i];

            std::string number = std::to_string(i);
            GLint uniform_var_loc1 = shdrpgm.GetLocation(("lightPositions[" + number + "]").c_str());
            GLint uniform_var_loc2 = shdrpgm.GetLocation(("lightColors[" + number + "]").c_str());
            GLint uModelToNDC_ = shdrpgm.GetLocation("uModelToNDC");
            GLint model_ = shdrpgm.GetLocation("model");
            GLuint view = shdrpgm.GetLocation("view");
            GLint projection = shdrpgm.GetLocation("projection");

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, albedo);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, metallic);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, roughness);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, ao);

            glm::mat4 mModelNDC;
            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            mModelNDC = _camera.projMtx * _camera.viewMtx * model;
            GLCall(glUniform3f(uniform_var_loc1, newPos.x, newPos.y, newPos.z));
            GLCall(glUniform3f(uniform_var_loc2, lightColors[i].x, lightColors[i].y, lightColors[i].z));
            glUniformMatrix4fv(uModelToNDC_, 1, GL_FALSE, glm::value_ptr(mModelNDC));
            glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
            glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));

            RenderSphere();
        }
    }

    void PBRManager::PBRScene()
    {
        engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SCENE);

        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
        auto& shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();
        GLuint cameraPos = shdrpgm.GetLocation("camPos");
        GLuint view = shdrpgm.GetLocation("view");
        GLint projection = shdrpgm.GetLocation("projection");
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
        GLCall(glUniform3f(cameraPos, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ()));
        shdrpgm.UnUse();

        UpdateLoop();
    }

    unsigned int PBRManager::loadTexture(char const* path)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
}