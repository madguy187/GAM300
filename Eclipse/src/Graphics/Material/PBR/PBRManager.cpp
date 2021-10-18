#include "pch.h"
#include "PBRManager.h"

namespace Eclipse
{
    void PBRManager::Init()
    {
        LoadMaterial("BambooWood");
        LoadMaterial("HardWood");
    }

    void PBRManager::CheckUniform(unsigned int ID , CameraComponent& In)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(In.camType));
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(In.camType));
        TransformComponent ModelTransform = engine->world.GetComponent<TransformComponent>(ID);

        auto& shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        GLuint metallic = shdrpgm.GetLocation("metallic");
        GLuint roughness = shdrpgm.GetLocation("roughness");
        GLint model_ = shdrpgm.GetLocation("model");
        GLuint view = shdrpgm.GetLocation("view");
        GLint projection = shdrpgm.GetLocation("projection");
        GLint HasTexture = shdrpgm.GetLocation("HasTexture");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, ModelTransform.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, ModelTransform.scale.ConvertToGlmVec3Type());

        GLCall(glUniform1i(HasTexture, true));
        GLCall(glUniform1f(metallic, 1.0f));
        GLCall(glUniform1f(roughness, 1.0f));
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));

        BindMaterial("HardWood", shdrpgm);
    }

    void PBRManager::LoadMaterial(std::string NameOfMaterial)
    {
        MaterialInstance NewMaterial(true, NameOfMaterial);
        NewMaterial.Albedo = loadTexture(("src/Assets/Materials/" + NameOfMaterial + "/albedo.png").c_str());
        NewMaterial.Normal = loadTexture(("src/Assets/Materials/" + NameOfMaterial + "/normal.png").c_str());
        NewMaterial.Metallic = loadTexture(("src/Assets/Materials/" + NameOfMaterial + "/metallic.png").c_str());
        NewMaterial.Roughness = loadTexture(("src/Assets/Materials/" + NameOfMaterial + "/roughness.png").c_str());
        NewMaterial.Ao = loadTexture(("src/Assets/Materials/" + NameOfMaterial + "/ao.png").c_str());
        AllMaterialInstances.insert({ NewMaterial.Name, std::make_shared<MaterialInstance>(NewMaterial) });
    }

    void PBRManager::BindMaterial(std::string MaterialName, Shader& In)
    {
        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_2D, AllMaterialInstances[MaterialName]->Albedo);

        glActiveTexture(GL_TEXTURE11);
        glBindTexture(GL_TEXTURE_2D, AllMaterialInstances[MaterialName]->Normal);

        glActiveTexture(GL_TEXTURE12);
        glBindTexture(GL_TEXTURE_2D, AllMaterialInstances[MaterialName]->Metallic);

        glActiveTexture(GL_TEXTURE13);
        glBindTexture(GL_TEXTURE_2D, AllMaterialInstances[MaterialName]->Roughness);

        glActiveTexture(GL_TEXTURE14);
        glBindTexture(GL_TEXTURE_2D, AllMaterialInstances[MaterialName]->Ao);

        In.setInt("albedoMap", 10);
        In.setInt("normalMap", 11);
        In.setInt("metallicMap", 12);
        In.setInt("roughnessMap", 13);
        In.setInt("aoMap", 14);
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
        GLint HasTexture = shdrpgm.GetLocation("HasTexture");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, { 10,10,10 });

        GLCall(glUniform1i(HasTexture, true));
        GLCall(glUniform1f(metallic, 1.0f));
        GLCall(glUniform1f(roughness, 1.0f));
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));

        BindMaterial("HardWood", shdrpgm);

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

        //UpdateLoop();
        //NonTexturedUpdateLoop();
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
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> uv;
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
                    normals.push_back(glm::vec3(xPos, yPos, zPos));
                    uv.push_back(glm::vec2(xSegment, ySegment));
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

    void PBRManager::NonTexturedUpdateLoop()
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));

        auto& shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        GLuint MetallicConstant = shdrpgm.GetLocation("MetallicConstant");
        GLuint RoughnessConstant = shdrpgm.GetLocation("RoughnessConstant");
        GLint uModelToNDC_ = shdrpgm.GetLocation("uModelToNDC");
        GLint model_ = shdrpgm.GetLocation("model");
        GLuint view = shdrpgm.GetLocation("view");
        GLint projection = shdrpgm.GetLocation("projection");
        GLuint AlbedoConstant = shdrpgm.GetLocation("AlbedoConstant");
        GLuint AoConstant = shdrpgm.GetLocation("AoConstant");
        GLuint HasTexture = shdrpgm.GetLocation("HasTexture");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, { 10,10,10 });

        GLCall(glUniform1i(HasTexture, false));
        GLCall(glUniform1f(MetallicConstant, 0.5f));
        GLCall(glUniform1f(RoughnessConstant, 0.5f));
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
        GLCall(glUniform3f(AlbedoConstant, 0.0, 1, 0));
        GLCall(glUniform1f(AoConstant, 1.0f));

        RenderSphere();
        shdrpgm.UnUse();
    }
}