#include "pch.h"
#include "Graphics/Material/MaterialEditorSettings/MaterialEditorSettings.h"
#include "Editor/Windows/MaterialEditor/MaterialEditor.h"

namespace Eclipse
{
    MaterialEditorSettings::MaterialEditorSettings()
    {
        // Light Source in MeshEditor
        LightPosition = ECVec3(15.0f, 15.0f, 15.0f);
        lightColor = ECVec3(1.0f, 1.0f, 1.0f);

        CreateSphere();
    }

    void MaterialEditorSettings::CreateMaterialInstance()
    {
        engine->gPBRManager->AllMaterialInstances.emplace(CurrentMaterial.Name.data(), std::make_unique<MaterialInstance>(CurrentMaterial));
        gMaterialCompiler.SerializeMaterials(CurrentMaterial);
    }

    void MaterialEditorSettings::BindMaterial(Shader& In, std::string MaterialName_)
    {
        if (SelectedIndex == 0)
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

            glActiveTexture(GL_TEXTURE15);
            glBindTexture(GL_TEXTURE_2D, CurrentMaterial.Height);
        }
        else
        {
            if (engine->gPBRManager->AllMaterialInstances.find(MaterialName_) != engine->gPBRManager->AllMaterialInstances.end())
            {
                glActiveTexture(GL_TEXTURE10);
                glBindTexture(GL_TEXTURE_2D, engine->gPBRManager->AllMaterialInstances[MaterialName_]->Albedo);

                glActiveTexture(GL_TEXTURE11);
                glBindTexture(GL_TEXTURE_2D, engine->gPBRManager->AllMaterialInstances[MaterialName_]->Normal);

                glActiveTexture(GL_TEXTURE12);
                glBindTexture(GL_TEXTURE_2D, engine->gPBRManager->AllMaterialInstances[MaterialName_]->Metallic);

                glActiveTexture(GL_TEXTURE13);
                glBindTexture(GL_TEXTURE_2D, engine->gPBRManager->AllMaterialInstances[MaterialName_]->Roughness);

                glActiveTexture(GL_TEXTURE14);
                glBindTexture(GL_TEXTURE_2D, engine->gPBRManager->AllMaterialInstances[MaterialName_]->Ao);

                glActiveTexture(GL_TEXTURE15);
                glBindTexture(GL_TEXTURE_2D, engine->gPBRManager->AllMaterialInstances[MaterialName_]->Height);
            }
        }

        In.setInt("albedoMap", 10);
        In.setInt("normalMap", 11);
        In.setInt("metallicMap", 12);
        In.setInt("roughnessMap", 13);
        In.setInt("aoMap", 14);
        In.setInt("displacement0", 15);
    }

    void MaterialEditorSettings::RenderMaterialScene()
    {
        if (engine->editorManager->GetEditorWindow<MaterialEditorWindow>()->IsVisible)
        {
            engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_MATERIALEDITOR);

            auto& shdrpgm = Graphics::shaderpgms["MaterialEditor"];
            shdrpgm.Use();

            auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::MaterialEditor_Camera));

            UpdateCamera(shdrpgm, _camera);
            UpdateLights(shdrpgm);
            UpdateCurrentMaterial(shdrpgm);
            RenderSphere();

            shdrpgm.UnUse();
        }
    }

    void MaterialEditorSettings::UpdateCurrentMaterial(Shader& shdrpgm)
    {
        GLuint MetallicConstant = shdrpgm.GetLocation("MetallicConstant");
        GLuint RoughnessConstant = shdrpgm.GetLocation("RoughnessConstant");
        GLint model_ = shdrpgm.GetLocation("model");
        GLuint AlbedoConstant = shdrpgm.GetLocation("AlbedoConstant");
        GLuint AoConstant = shdrpgm.GetLocation("AoConstant");
        GLint HasInstance = shdrpgm.GetLocation("HasInstance");
        GLint BaseReflectivity = shdrpgm.GetLocation("BaseReflectivity");
        GLint NormalMap_ = shdrpgm.GetLocation("NormalMap");
        GLint HeightScale_ = shdrpgm.GetLocation("HeightScale");
        GLint SurfaceColour_ = shdrpgm.GetLocation("SurfaceColour");
        GLint EmissiveMaterial_ = shdrpgm.GetLocation("EmissiveMaterial");
        GLint EmissiveColour_ = shdrpgm.GetLocation("EmissiveColour");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3{ 0.0f });
        model = glm::rotate(model, glm::radians(Rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3{ 5.0f });

        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));

        // Emissive
        GLCall(glUniform1i(EmissiveMaterial_, CurrentMaterial.EmissiveMaterial));
        GLCall(glUniform3f(EmissiveColour_, CurrentMaterial.EmissiveColour.getX(), CurrentMaterial.EmissiveColour.getY(), CurrentMaterial.EmissiveColour.getZ()));

        // When Material is selected
        GLCall(glUniform3f(SurfaceColour_, CurrentMaterial.SurfaceColour.getX(), CurrentMaterial.SurfaceColour.getY(), CurrentMaterial.SurfaceColour.getZ()));
        GLCall(glUniform1i(NormalMap_, CurrentMaterial.IsNormalMap));
        GLCall(glUniform1f(HeightScale_, CurrentMaterial.HeightScale));
        GLCall(glUniform1i(HasInstance, CurrentMaterial.HasTexture));
        GLCall(glUniform3f(AlbedoConstant, CurrentMaterial.AlbedoConstant.getX(), CurrentMaterial.AlbedoConstant.getY(), CurrentMaterial.AlbedoConstant.getZ()));
        GLCall(glUniform1f(AoConstant, CurrentMaterial.AoConstant));
        GLCall(glUniform1f(MetallicConstant, CurrentMaterial.MetallicConstant));
        GLCall(glUniform1f(RoughnessConstant, CurrentMaterial.RoughnessConstant));
        GLCall(glUniform3f(BaseReflectivity, CurrentMaterial.BaseReflectivity.getX(), CurrentMaterial.BaseReflectivity.getY(), CurrentMaterial.BaseReflectivity.getZ()));

        BindMaterial(shdrpgm, CurrentMaterial.Name.data());
    }

    void MaterialEditorSettings::ClearCurrentMaterial()
    {
        MaterialInstance NewMaterial;
        CurrentMaterial = NewMaterial;
    }

    void MaterialEditorSettings::ClearTextureFields()
    {
        AlbedoTexture.clear();
        NormalTexture.clear();
        MetallicTexture.clear();
        RoughnessTexture.clear();
        AoTexture.clear();
        HeightTexture.clear();
        MaterialName.clear();
    }

    void MaterialEditorSettings::UpdateLights(Shader& MaterialEditorShader)
    {
        std::string number = std::to_string(0);
        GLint uniform_var_loc1 = MaterialEditorShader.GetLocation(("pointLights[" + number + "].position").c_str());
        GLint uniform_var_loc2 = MaterialEditorShader.GetLocation(("pointLights[" + number + "].lightColor").c_str());

        GLCall(glUniform3f(uniform_var_loc1, LightPosition.getX(), LightPosition.getY(), LightPosition.getZ()));
        GLCall(glUniform3f(uniform_var_loc2, 2.0f, 2.0f, 2.0f));
    }

    void MaterialEditorSettings::UpdateCamera(Shader& MaterialEditorShader, CameraComponent& MeshEditorCamera)
    {
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(MeshEditorCamera.camType));

        GLuint cameraPos = MaterialEditorShader.GetLocation("camPos");
        GLuint view = MaterialEditorShader.GetLocation("view");
        GLint projection = MaterialEditorShader.GetLocation("projection");

        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(MeshEditorCamera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(MeshEditorCamera.viewMtx));
        GLCall(glUniform3f(cameraPos, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ()));
    }

    void MaterialEditorSettings::CreateSphere()
    {
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
            indexCount = static_cast<unsigned int>(indices.size());

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
    }

    void MaterialEditorSettings::RenderSphere()
    {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
    }
}