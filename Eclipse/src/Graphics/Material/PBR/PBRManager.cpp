#include "pch.h"
#include "PBRManager.h"

namespace Eclipse
{
    PBRManager::PBRManager()
    {
        Init();
    }

    void PBRManager::Init()
    {
        InitialiseBaseReflectivity();
        LoadAllTextures();

        gMaterialEditorSettings = std::make_unique<MaterialEditorSettings>();

        LoadMaterial("BambooWood");
        LoadMaterial("HardWood");
    }

    void PBRManager::InitialiseBaseReflectivity()
    {
        BaseReflectivity.emplace("Water", ECVec3{ 0.02,0.02,0.02 });
        BaseReflectivity.emplace("Plastic(Low)", ECVec3{ 0.03,0.03,0.03 });
        BaseReflectivity.emplace("Plastic(High)", ECVec3{ 0.05,0.05,0.05 });
        BaseReflectivity.emplace("Glass(High)", ECVec3{ 0.08,0.08,0.08 });
        BaseReflectivity.emplace("Diamond", ECVec3{ 0.17,0.17,0.17 });
        BaseReflectivity.emplace("Iron", ECVec3{ 0.56,0.56,0.56 });
        BaseReflectivity.emplace("Copper", ECVec3{ 0.95,0.64,0.54 });
        BaseReflectivity.emplace("Gold", ECVec3{ 1.00,0.71,0.29 });
        BaseReflectivity.emplace("Aluminium", ECVec3{ 0.91,0.92,0.92 });
        BaseReflectivity.emplace("Silver", ECVec3{ 0.95,0.93,0.88 });
    }

    void PBRManager::GenerateMaterialTexture(std::string FolderName, std::string TextureName)
    {
        if (TextureName.find("albedo") != std::string::npos)
        {
            //Found Loaded Texture
            if (AllMaterialTextures.find(FolderName) != AllMaterialTextures.end())
            {
                gMaterialEditorSettings->CurrentMaterial.Albedo = AllMaterialTextures[FolderName][MaterialType::MT_ALBEDO];
            }
        }
        else if (TextureName.find("normal") != std::string::npos)
        {
            //Found Loaded Texture
            if (AllMaterialTextures.find(FolderName) != AllMaterialTextures.end())
            {
                gMaterialEditorSettings->CurrentMaterial.Normal = AllMaterialTextures[FolderName][MaterialType::MT_NORMAL];
            }
        }
        else if (TextureName.find("metallic") != std::string::npos)
        {
            //Found Loaded Texture
            if (AllMaterialTextures.find(FolderName) != AllMaterialTextures.end())
            {
                gMaterialEditorSettings->CurrentMaterial.Metallic = AllMaterialTextures[FolderName][MaterialType::MT_METALLIC];
            }
        }
        else if (TextureName.find("roughness") != std::string::npos)
        {
            //Found Loaded Texture
            if (AllMaterialTextures.find(FolderName) != AllMaterialTextures.end())
            {
                gMaterialEditorSettings->CurrentMaterial.Roughness = AllMaterialTextures[FolderName][MaterialType::MT_ROUGHNESS];
            }
        }
        else if (TextureName.find("ao") != std::string::npos)
        {
            //Found Loaded Texture
            if (AllMaterialTextures.find(FolderName) != AllMaterialTextures.end())
            {
                gMaterialEditorSettings->CurrentMaterial.Ao = AllMaterialTextures[FolderName][MaterialType::MT_AO];
            }
        }
    }

    void PBRManager::LoadAllTextures()
    {
        for (auto& dirEntry : std::filesystem::directory_iterator("src//Assets//Materials"))
        {
            const auto& path = dirEntry.path();
            auto relativePath = relative(path, "src//");
            std::string FolderName = relativePath.filename().string();

            std::string GoIntoMaterialFolder = ("src/Assets/Materials/" + FolderName);

            for (auto& dirEntry : std::filesystem::directory_iterator(GoIntoMaterialFolder))
            {
                const auto& MaterialTextureFiles = dirEntry.path();
                auto relativePath = relative(MaterialTextureFiles, "src//");
                std::string MaterialTextureFileName = relativePath.filename().string();

                if (MaterialTextureFileName.find("albedo.png") != std::string::npos)
                {
                    unsigned int TextureID = loadTexture((GoIntoMaterialFolder + "/albedo.png").c_str());
                    AllMaterialTextures[FolderName][MaterialType::MT_ALBEDO] = TextureID;
                }
                else if (MaterialTextureFileName.find("normal.png") != std::string::npos)
                {
                    unsigned int TextureID = loadTexture((GoIntoMaterialFolder + "/normal.png").c_str());
                    AllMaterialTextures[FolderName][MaterialType::MT_NORMAL] = TextureID;
                }
                else if (MaterialTextureFileName.find("metallic.png") != std::string::npos)
                {
                    unsigned int TextureID = loadTexture((GoIntoMaterialFolder + "/metallic.png").c_str());
                    AllMaterialTextures[FolderName][MaterialType::MT_METALLIC] = TextureID;
                }
                else if (MaterialTextureFileName.find("roughness.png") != std::string::npos)
                {
                    unsigned int TextureID = loadTexture((GoIntoMaterialFolder + "/roughness.png").c_str());
                    AllMaterialTextures[FolderName][MaterialType::MT_ROUGHNESS] = TextureID;
                }
                else if (MaterialTextureFileName.find("ao.png") != std::string::npos)
                {
                    unsigned int TextureID = loadTexture((GoIntoMaterialFolder + "/ao.png").c_str());
                    AllMaterialTextures[FolderName][MaterialType::MT_AO] = TextureID;
                }
            }
        }
    }

    void PBRManager::NonTexturedUniform(unsigned int ID, CameraComponent& In)
    {
        auto& MaterialCom = engine->world.GetComponent<MaterialComponent>(ID);

        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(In.camType));
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(In.camType));
        TransformComponent ModelTransform = engine->world.GetComponent<TransformComponent>(ID);

        auto& shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

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
        model = glm::translate(model, ModelTransform.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, ModelTransform.scale.ConvertToGlmVec3Type());

        GLCall(glUniform1i(HasInstance, AllMaterialInstances[MaterialCom.MaterialInstanceName]->HasTexture));
        GLCall(glUniform3f(AlbedoConstant,
            AllMaterialInstances[MaterialCom.MaterialInstanceName]->AlbedoConstant.getX(),
            AllMaterialInstances[MaterialCom.MaterialInstanceName]->AlbedoConstant.getY(),
            AllMaterialInstances[MaterialCom.MaterialInstanceName]->AlbedoConstant.getZ()));
        GLCall(glUniform1f(AoConstant, AllMaterialInstances[MaterialCom.MaterialInstanceName]->AoConstant));
        GLCall(glUniform1f(MetallicConstant, AllMaterialInstances[MaterialCom.MaterialInstanceName]->MetallicConstant));
        GLCall(glUniform1f(RoughnessConstant, AllMaterialInstances[MaterialCom.MaterialInstanceName]->RoughnessConstant));
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projection1, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view1, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
        GLCall(glUniform3f(BaseReflectivity,
            AllMaterialInstances[MaterialCom.MaterialInstanceName]->BaseReflectivity.getX(),
            AllMaterialInstances[MaterialCom.MaterialInstanceName]->BaseReflectivity.getY(),
            AllMaterialInstances[MaterialCom.MaterialInstanceName]->BaseReflectivity.getZ()));
    }

    void PBRManager::TexturedUniform(unsigned int ID, CameraComponent& In)
    {
        auto& MaterialCom = engine->world.GetComponent<MaterialComponent>(ID);

        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(In.camType));
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(In.camType));
        TransformComponent ModelTransform = engine->world.GetComponent<TransformComponent>(ID);

        auto& shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        GLint uModelToNDC_ = shdrpgm.GetLocation("uModelToNDC");
        GLint model_ = shdrpgm.GetLocation("model");
        GLuint view1 = shdrpgm.GetLocation("view");
        GLint projection1 = shdrpgm.GetLocation("projection");
        GLint HasInstance = shdrpgm.GetLocation("HasInstance");
        GLint BaseReflectivity = shdrpgm.GetLocation("BaseReflectivity");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, ModelTransform.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(ModelTransform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, ModelTransform.scale.ConvertToGlmVec3Type());

        GLCall(glUniform1i(HasInstance, AllMaterialInstances[MaterialCom.MaterialInstanceName]->HasTexture));
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projection1, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view1, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
        GLCall(glUniform3f(BaseReflectivity,
            AllMaterialInstances[MaterialCom.MaterialInstanceName]->BaseReflectivity.getX(),
            AllMaterialInstances[MaterialCom.MaterialInstanceName]->BaseReflectivity.getY(),
            AllMaterialInstances[MaterialCom.MaterialInstanceName]->BaseReflectivity.getZ()));

        BindMaterial(MaterialCom.MaterialInstanceName, shdrpgm);
    }

    void PBRManager::LoadMaterial(std::string NameOfMaterial)
    {
        MaterialInstance NewMaterial(true, NameOfMaterial);
        NewMaterial.Albedo = loadTexture(("src/Assets/Materials/" + NameOfMaterial + "/albedo.png").c_str());
        NewMaterial.Normal = loadTexture(("src/Assets/Materials/" + NameOfMaterial + "/normal.png").c_str());
        NewMaterial.Metallic = loadTexture(("src/Assets/Materials/" + NameOfMaterial + "/metallic.png").c_str());
        NewMaterial.Roughness = loadTexture(("src/Assets/Materials/" + NameOfMaterial + "/roughness.png").c_str());
        NewMaterial.Ao = loadTexture(("src/Assets/Materials/" + NameOfMaterial + "/ao.png").c_str());
        AllMaterialInstances.insert({ NewMaterial.MeshName.data(), std::make_unique<MaterialInstance>(NewMaterial) });
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

    void PBRManager::SetAlbedoConstant(Shader& In, glm::vec4& AlbedoValue)
    {
        GLuint AlbedoConstant = In.GetLocation("AlbedoConstant");
        glUniform3f(AlbedoConstant, AlbedoValue.r, AlbedoValue.g, AlbedoValue.b);
    }

    void PBRManager::SetMetallicConstant(Shader& In, float metallicValue)
    {
        GLuint MetallicConstant = In.GetLocation("MetallicConstant");
        GLCall(glUniform1f(MetallicConstant, metallicValue));
    }

    void PBRManager::SetRoughnessConstant(Shader& In, float RoughnessValue)
    {
        GLuint RoughnessConstant = In.GetLocation("RoughnessConstant");
        GLCall(glUniform1f(RoughnessConstant, RoughnessValue));
    }

    void PBRManager::SetInstanceFlag(Shader& In, bool instanceflag)
    {
        GLuint HasTexture = In.GetLocation("HasInstance");
        GLCall(glUniform1i(HasTexture, instanceflag));
    }

    void PBRManager::SetAOConstant(Shader& In, float AOValue)
    {
        GLuint AoConstant = In.GetLocation("AoConstant");
        GLCall(glUniform1f(AoConstant, AOValue));
    }

    void PBRManager::UnBindMetallicTexture(Shader& In)
    {
        In.setInt("metallicMap", 0);
    }

    void PBRManager::UnBindRoughnessTexture(Shader& In)
    {
        In.setInt("roughnessMap", 0);
    }

    void PBRManager::UnBindAOTexture(Shader& In)
    {
        In.setInt("aoMap", 0);
    }

    void PBRManager::UpdateLoop()
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));

        auto& shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        GLint uModelToNDC_ = shdrpgm.GetLocation("uModelToNDC");
        GLint model_ = shdrpgm.GetLocation("model");
        GLuint view = shdrpgm.GetLocation("view");
        GLint projection = shdrpgm.GetLocation("projection");
        GLint HasInstance = shdrpgm.GetLocation("HasInstance");
        GLint Constants = shdrpgm.GetLocation("Constants");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, { 10,10,10 });

        GLCall(glUniform1i(HasInstance, true));
        GLCall(glUniform1i(Constants, false));
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));

        BindMaterial(gMaterialEditorSettings->MaterialName, shdrpgm);

        gMaterialEditorSettings->RenderSphere();
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
                format = GL_SRGB;

            //std::cout << "Number of Components " << nrComponents << std::endl;

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

        gMaterialEditorSettings->RenderSphere();
        shdrpgm.UnUse();
    }

    void PBRManager::Clear(std::string& TextureName, MaterialType In)
    {
        switch (In)
        {
        case MaterialType::MT_ALBEDO:
        {
            if (TextureName.find("albedo") == std::string::npos)
                gMaterialEditorSettings->CurrentMaterial.Albedo = 0;
        }
        break;

        case MaterialType::MT_NORMAL:
        {
            if (TextureName.find("normal") == std::string::npos)
                gMaterialEditorSettings->CurrentMaterial.Normal = 0;
        }
        break;

        case MaterialType::MT_METALLIC:
        {
            if (TextureName.find("metallic") == std::string::npos)
                gMaterialEditorSettings->CurrentMaterial.Metallic = 0;
        }
        break;

        case MaterialType::MT_ROUGHNESS:
        {
            if (TextureName.find("roughness") == std::string::npos)
                gMaterialEditorSettings->CurrentMaterial.Roughness = 0;
        }
        break;

        case MaterialType::MT_AO:
        {
            if (TextureName.find("ao") == std::string::npos)
                gMaterialEditorSettings->CurrentMaterial.Ao = 0;
        }
        break;
        }
    }

    void PBRManager::BloomScene()
    {
        std::vector<glm::vec3> lightPositions;
        lightPositions.push_back(glm::vec3(0.0f, 5.5f, 1.5f));
        lightPositions.push_back(glm::vec3(4.0f, 5.5f, 7.0f));
        lightPositions.push_back(glm::vec3(3.0f, 5.5f, 1.0f));
        lightPositions.push_back(glm::vec3(.8f, 9.4f, 1.0f));

        std::vector<glm::vec3> lightColors;
        lightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
        lightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
        lightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
        lightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));

        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
        auto& campos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));

        engine->MaterialManager.DoNotUpdateStencil();
        engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SCENE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        auto& shdrpgm = Graphics::shaderpgms["Bloom"];
        shdrpgm.Use();
        shdrpgm.setMat4("projection", _camera.projMtx);
        shdrpgm.setMat4("view", _camera.viewMtx);
        shdrpgm.setInt("diffuseTexture", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glm::mat4 model = glm::mat4(1.0f);

        for (unsigned int i = 0; i < lightPositions.size(); i++)
        {
            shdrpgm.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
            shdrpgm.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        }

        glm::vec3 hi = campos.position.ConvertToGlmVec3Type();
        shdrpgm.setVec3("viewPos", hi);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0));
        model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
        shdrpgm.setMat4("model", model);
        renderCube();


        auto& shdrpgm2 = Graphics::shaderpgms["ShaderLights"];
        shdrpgm2.Use();
        shdrpgm2.setMat4("projection", _camera.projMtx);
        shdrpgm2.setMat4("view", _camera.viewMtx);

        for (unsigned int i = 0; i < lightPositions.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(lightPositions[i]));
            model = glm::scale(model, glm::vec3(0.25f));
            shdrpgm2.setMat4("model", model);
            shdrpgm2.setVec3("lightColor", lightColors[i]);
            
            gMaterialEditorSettings->RenderSphere();
        }
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SCENE);
        //engine->MaterialManager.DoNotUpdateStencil();
        //auto& shdrpgm4 = Graphics::shaderpgms["BloomFinal"];
        //shdrpgm4.Use();
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(FrameBufferMode::FBM_SCENE));
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, engine->GraphicsManager.Pong->pingpongColorbuffers[!horizontal]);
        //shdrpgm4.setInt("bloom", bloom);
        //shdrpgm4.setFloat("exposure", 1.0f);
        //shdrpgm4.setInt("scene", 0);
        //shdrpgm4.setInt("bloomBlur", 1);
        //renderQuad();
        //engine->MaterialManager.StencilBufferClear();

        engine->MaterialManager.DoNotUpdateStencil();
    }

    void PBRManager::last()
    {
        auto& shdrpgm3 = Graphics::shaderpgms["Blur"];
        shdrpgm3.Use();
        horizontal = true, first_iteration = true;
        unsigned int amount = 10;
        shdrpgm3.setInt("image", 0);

        engine->MaterialManager.DoNotUpdateStencil();
        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, engine->GraphicsManager.Pong->pingpongFBO[horizontal]);
            shdrpgm3.setInt("horizontal", horizontal);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, first_iteration ?
                engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_SCENE)->m_data.ColorBuffers[1] :
                engine->GraphicsManager.Pong->pingpongColorbuffers[!horizontal]);

            renderQuad();

            horizontal = !horizontal;

            if (first_iteration)
                first_iteration = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SCENE);
        engine->MaterialManager.DoNotUpdateStencil();

        //glEnable(GL_BLEND);
        //glBlendFunc(GL_DST_COLOR, GL_ZERO);
        ////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_DEPTH_TEST);
        //glDisable(GL_CULL_FACE);

        auto& shdrpgm4 = Graphics::shaderpgms["BloomFinal"];
        shdrpgm4.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(FrameBufferMode::FBM_SCENE));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, engine->GraphicsManager.Pong->pingpongColorbuffers[!horizontal]);
        shdrpgm4.setInt("bloom", bloom);
        shdrpgm4.setFloat("exposure", 1.0f);
        shdrpgm4.setInt("scene", 0);
        shdrpgm4.setInt("bloomBlur", 1);
        renderQuad();
        engine->MaterialManager.StencilBufferClear();
    }

    void PBRManager::renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

    void PBRManager::renderCube()
    {
        // initialize (if necessary)
        if (cubeVAO == 0)
        {
            float vertices[] = {
                // back face
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
                // front face
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
                 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
                // left face
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                // right face
                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
                 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
                 // bottom face
                 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
                  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
                  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
                  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
                 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
                 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
                 // top face
                 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
                  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
                  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
                  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
                 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
                 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
            };
            glGenVertexArrays(1, &cubeVAO);
            glGenBuffers(1, &cubeVBO);

            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(cubeVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
}