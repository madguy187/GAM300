#include "pch.h"
#include "PBRManager.h"

namespace Eclipse
{
    void PBRManager::SetShader()
    {
        MaterialInstance brick;
        Texture Tex1("src/Assets/Materials/RustedIron/albedo.png");
        Texture Tex2("src/Assets/Materials/RustedIron/normal.png");
        Texture Tex3("src/Assets/Materials/RustedIron/metallic.png");
        Texture Tex4("src/Assets/Materials/RustedIron/roughness.png");
        Texture Tex5("src/Assets/Materials/RustedIron/ao.png");

        brick.Name = "Brick";
        brick.albedo = Tex1.GetHandle();
        brick.normal = Tex2.GetHandle();
        brick.metallic = Tex3.GetHandle();
        brick.roughness = Tex4.GetHandle();
        brick.ao = Tex5.GetHandle();
        MaterialInstances.push_back(brick);

        MaterialInstance BambooWood;
        Texture Tex6("src/Assets/Materials/BambooWood/albedo.png");
        Texture Tex7("src/Assets/Materials/BambooWood/normal.png");
        Texture Tex8("src/Assets/Materials/BambooWood/metal.png");
        Texture Tex9("src/Assets/Materials/BambooWood/roughness.png");
        Texture Tex10("src/Assets/Materials/BambooWood/ao.png");
        BambooWood.Name = "BambooWood";
        BambooWood.albedo = Tex6.GetHandle();
        BambooWood.normal = Tex7.GetHandle();
        BambooWood.metallic = Tex8.GetHandle();
        BambooWood.roughness = Tex9.GetHandle();
        BambooWood.ao = Tex10.GetHandle();
        MaterialInstances.push_back(BambooWood);

        MaterialInstance Granite;
        Texture Tex11("src/Assets/Materials/GrayGranite/albedo.png");
        Texture Tex12("src/Assets/Materials/GrayGranite/normal.png");
        Texture Tex13("src/Assets/Materials/GrayGranite/metallic.png");
        Texture Tex14("src/Assets/Materials/GrayGranite/roughness.png");
        Texture Tex15("src/Assets/Materials/GrayGranite/ao.png");
        Granite.Name = "Granite";
        Granite.albedo = Tex11.GetHandle();
        Granite.normal = Tex12.GetHandle();
        Granite.metallic = Tex13.GetHandle();
        Granite.roughness = Tex14.GetHandle();
        Granite.ao = Tex15.GetHandle();
        MaterialInstances.push_back(Granite);
    }

    void PBRManager::CheckUniform(unsigned int ID)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
        TransformComponent ModelTransform = engine->world.GetComponent<TransformComponent>(ID);

        auto& shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        GLuint metallic = shdrpgm.GetLocation("metallic");
        GLuint roughness = shdrpgm.GetLocation("roughness");
        GLint model_ = shdrpgm.GetLocation("model");
        GLuint view = shdrpgm.GetLocation("view");
        GLint projection = shdrpgm.GetLocation("projection");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, ModelTransform.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, ModelTransform.scale.ConvertToGlmVec3Type());

        GLCall(glUniform1f(metallic, 1.0f));
        GLCall(glUniform1f(roughness, 1.0f));
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));

        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_2D, MaterialInstances[0].albedo);

        glActiveTexture(GL_TEXTURE11);
        glBindTexture(GL_TEXTURE_2D, MaterialInstances[0].normal);

        glActiveTexture(GL_TEXTURE12);
        glBindTexture(GL_TEXTURE_2D, MaterialInstances[0].metallic);

        glActiveTexture(GL_TEXTURE13);
        glBindTexture(GL_TEXTURE_2D, MaterialInstances[0].roughness);

        glActiveTexture(GL_TEXTURE14);
        glBindTexture(GL_TEXTURE_2D, MaterialInstances[0].ao);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        shdrpgm.setInt("albedoMap", 10);
        shdrpgm.setInt("normalMap", 11);
        shdrpgm.setInt("metallicMap", 12);
        shdrpgm.setInt("roughnessMap", 13);
        shdrpgm.setInt("aoMap", 14);

    }

    void PBRManager::RenderSphere()
    {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
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

        auto& shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        GLuint metallic = shdrpgm.GetLocation("metallic");
        GLuint roughness = shdrpgm.GetLocation("roughness");
        GLint uModelToNDC_ = shdrpgm.GetLocation("uModelToNDC");
        GLint model_ = shdrpgm.GetLocation("model");
        GLuint view = shdrpgm.GetLocation("view");
        GLint projection = shdrpgm.GetLocation("projection");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, { 5,5,5 });

        GLCall(glUniform1f(metallic, 0.5f));
        GLCall(glUniform1f(roughness, 0.0f));
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));

        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_2D, MaterialInstances[2].albedo);

        glActiveTexture(GL_TEXTURE11);
        glBindTexture(GL_TEXTURE_2D, MaterialInstances[2].normal);

        glActiveTexture(GL_TEXTURE12);
        glBindTexture(GL_TEXTURE_2D, MaterialInstances[2].metallic);

        glActiveTexture(GL_TEXTURE13);
        glBindTexture(GL_TEXTURE_2D, MaterialInstances[2].roughness);

        glActiveTexture(GL_TEXTURE14);
        glBindTexture(GL_TEXTURE_2D, MaterialInstances[2].ao);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        shdrpgm.setInt("albedoMap", 10);
        shdrpgm.setInt("normalMap", 11);
        shdrpgm.setInt("metallicMap", 12);
        shdrpgm.setInt("roughnessMap", 13);
        shdrpgm.setInt("aoMap", 14);

        RenderSphere();
        shdrpgm.UnUse();
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
}