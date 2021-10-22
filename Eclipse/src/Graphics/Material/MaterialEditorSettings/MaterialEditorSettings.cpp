#include "pch.h"
#include "Graphics/Material/MaterialEditorSettings/MaterialEditorSettings.h"

namespace Eclipse
{
    MaterialEditorSettings::MaterialEditorSettings()
    {
        // Light Source in MeshEditor
        LightPosition = ECVec3(10.0f, 10.0f, 10.0f);
        lightColor = ECVec3(300.0f, 300.0f, 300.0f);
    }

    void MaterialEditorSettings::CreateMaterialInstance()
    {
        engine->gPBRManager->AllMaterialInstances.emplace(CurrentMaterial.MeshName.data(), std::make_unique<MaterialInstance>(CurrentMaterial));

        std::string hi = CurrentMaterial.MeshName.data();

        MaterialFileWrite.open("src/Assets/MaterialInstances/" + hi + ".mat",
            std::ios_base::out |
            std::ios_base::trunc |
            std::ios_base::binary);

        if (MaterialFileWrite.fail())
        {
            return;
        }

        MaterialInstance A = CurrentMaterial;
        A.Ao = 3;
        MaterialFileWrite.write(reinterpret_cast<const char*>(&A), offsetof(MaterialInstance, Stopper));
        MaterialFileWrite.close();

        MaterialFileRead.open("src/Assets/MaterialInstances/" + hi + ".mat", std::ios::in | std::ios::binary);

        if (MaterialFileRead.fail())
        {
            return;
        }

        MaterialInstance B;
        MaterialFileRead.read(reinterpret_cast<char*>(&B), offsetof(MaterialInstance, Stopper));
        MaterialFileRead.close();
    }

    void MaterialEditorSettings::BindMaterial(std::string MaterialName, Shader& In)
    {
        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_2D, CurrentMaterial.Albedo);

        glActiveTexture(GL_TEXTURE11);
        glBindTexture(GL_TEXTURE_2D, CurrentMaterial.Normal);

        glActiveTexture(GL_TEXTURE12);
        glBindTexture(GL_TEXTURE_2D, CurrentMaterial.Metallic);

        glActiveTexture(GL_TEXTURE13);
        glBindTexture(GL_TEXTURE_2D, CurrentMaterial.Roughness);

        glActiveTexture(GL_TEXTURE14);
        glBindTexture(GL_TEXTURE_2D, CurrentMaterial.Ao);

        In.setInt("albedoMap", 10);
        In.setInt("normalMap", 11);
        In.setInt("metallicMap", 12);
        In.setInt("roughnessMap", 13);
        In.setInt("aoMap", 14);
    }

    void MaterialEditorSettings::UpdateCurrentMaterial()
    {
        engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_MESHEDITOR);

        auto& shdrpgm = Graphics::shaderpgms["MaterialEditor"];
        shdrpgm.Use();

        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::MeshEditor_Camera));

        UpdateCamera(shdrpgm, _camera);
        UpdateLights(shdrpgm);

        GLuint MetallicConstant = shdrpgm.GetLocation("MetallicConstant");
        GLuint RoughnessConstant = shdrpgm.GetLocation("RoughnessConstant");
        GLint uModelToNDC_ = shdrpgm.GetLocation("uModelToNDC");
        GLint model_ = shdrpgm.GetLocation("model");
        GLuint view1 = shdrpgm.GetLocation("view");
        GLint projection1 = shdrpgm.GetLocation("projection");
        GLuint AlbedoConstant = shdrpgm.GetLocation("AlbedoConstant");
        GLuint AoConstant = shdrpgm.GetLocation("AoConstant");
        GLint HasInstance = shdrpgm.GetLocation("HasInstance");
        GLint BaseReflectivity = shdrpgm.GetLocation("BaseReflectivity");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, { 5,5,5 });

        GLCall(glUniform1i(HasInstance, CurrentMaterial.HasTexture));
        GLCall(glUniform3f(AlbedoConstant, CurrentMaterial.AlbedoConstant.getX(), CurrentMaterial.AlbedoConstant.getY(), CurrentMaterial.AlbedoConstant.getZ()));
        GLCall(glUniform1f(AoConstant, CurrentMaterial.AoConstant));
        GLCall(glUniform1f(MetallicConstant, CurrentMaterial.MetallicConstant));
        GLCall(glUniform1f(RoughnessConstant, CurrentMaterial.RoughnessConstant));
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projection1, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view1, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
        GLCall(glUniform3f(BaseReflectivity, CurrentMaterial.BaseReflectivity.getX(), CurrentMaterial.BaseReflectivity.getY(), CurrentMaterial.BaseReflectivity.getZ()));

        BindMaterial("HardWood", shdrpgm);

        RenderSphere();
        shdrpgm.UnUse();
    }

    void MaterialEditorSettings::UpdateLights(Shader& MaterialEditorShader)
    {
        std::string number = std::to_string(0);
        GLint uniform_var_loc1 = MaterialEditorShader.GetLocation(("pointLights[" + number + "].position").c_str());
        GLint uniform_var_loc2 = MaterialEditorShader.GetLocation(("pointLights[" + number + "].lightColor").c_str());

        GLCall(glUniform3f(uniform_var_loc1, LightPosition.getX(), LightPosition.getY(), LightPosition.getZ()));
        GLCall(glUniform3f(uniform_var_loc2, 300.0f, 300.0f, 300.0f));
    }

    void MaterialEditorSettings::UpdateCamera(Shader& MaterialEditorShader, CameraComponent& MeshEditorCamera)
    {
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::MeshEditor_Camera));

        GLuint cameraPos = MaterialEditorShader.GetLocation("camPos");
        GLuint view = MaterialEditorShader.GetLocation("view");
        GLint projection = MaterialEditorShader.GetLocation("projection");

        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(MeshEditorCamera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(MeshEditorCamera.viewMtx));
        GLCall(glUniform3f(cameraPos, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ()));
    }

    void MaterialEditorSettings::RenderSphere()
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
}