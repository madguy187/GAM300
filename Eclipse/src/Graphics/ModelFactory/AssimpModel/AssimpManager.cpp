#include "pch.h"
#include "AssimpManager.h"

namespace Eclipse
{
    void AssimpModelManager::Init()
    {
        LoadModels("src//Assets//ASSModels");
        PrintLoadedModels();
    }

    void AssimpModelManager::LoadModels(const std::string& modelFile)
    {
        for (auto& dirEntry : std::filesystem::directory_iterator(modelFile))
        {
            const auto& path = dirEntry.path();
            auto relativePath = relative(path, "src//");
            std::string FolderName = relativePath.filename().string();

            std::string GoIntoModelFolder = ("src/Assets/ASSModels/" + FolderName);

            for (auto& dirEntry : std::filesystem::directory_iterator(GoIntoModelFolder))
            {
                const auto& FbxOrGltf = dirEntry.path();
                auto relativePath = relative(FbxOrGltf, "src//");
                std::string FbxOrGltfName = relativePath.filename().string();

                if (FbxOrGltfName.find("gltf") != std::string::npos || FbxOrGltfName.find("fbx") != std::string::npos)
                {
                    std::string PathName = ("src/Assets/ASSModels/" + FolderName + "/" + FbxOrGltfName).c_str();

                    std::unique_ptr<AssimpModel> ptr(new AssimpModel(false));
                    ptr->SetProperties(FolderName, ModelType::MT_ANIMAL);
                    ptr->LoadAssimpModel(PathName);

                    AssimpLoadedModels.emplace(FolderName, std::move(ptr));
                    ModelMap.emplace(FolderName, PathName);
                }
            }
        }

        int i = 0;

        // First file path so i shd be src/assets/assmodels
        //for (auto& dirEntry : std::filesystem::directory_iterator("src//Assets//ASSModels"))
        //{
        //    const auto& path = dirEntry.path();
        //    auto relativePath = relative(path, "src//");
        //    std::string FolderName = relativePath.filename().string(); // Return me the name of the folder of my model
        //    std::string PathName = ("src/Assets/ASSModels/" + FolderName + "/").c_str();

        //    for (auto& dirEntry : std::filesystem::directory_iterator(PathName))
        //    {
        //        const auto& path = dirEntry.path();
        //        auto relativePath = relative(path, "src//");
        //        std::string NextFolderName = relativePath.filename().string();

        //        if (NextFolderName == "textures")
        //        {
        //            std::string InsideModelFolder = ("src/Assets/ASSModels/" + FolderName).c_str();
        //            std::string InsideTextures = ("src/Assets/ASSModels/" + FolderName + "/" + NextFolderName).c_str();

        //            unsigned int Index = 0;

        //            std::unique_ptr<Texture> LastTracked;

        //            std::string TexturePath;

        //            for (auto& dirEntry : std::filesystem::directory_iterator(InsideTextures))
        //            {
        //                const auto& path = dirEntry.path();
        //                auto relativePath = relative(path, "src//");
        //                std::string TextureName = relativePath.filename().string();

        //                 TexturePath = NextFolderName + "/" + TextureName;
        //                // If i find the first base colour , i insert.
        //                if (TextureName.find("baseColor") != std::string::npos)
        //                {
        //                    Texture tex(InsideModelFolder, TexturePath, aiTextureType_DIFFUSE);
        //                    std::unique_ptr<Texture> ptr(new Texture(tex));
        //                    ptr->Load(false);
        //                    engine->AssimpManager.InsertTextures(FolderName, std::move(ptr), Index);

        //                    std::unique_ptr<Texture> ptr2(new Texture(tex));
        //                    engine->AssimpManager.InsertTextures(FolderName, std::move(ptr2), Index);

        //                    LastTracked = std::unique_ptr<Texture>((new Texture(tex)));

        //                    Index++;
        //                }
        //            }

        //            if (AssimpLoadedModels[FolderName]->GetMesh().size() != Index)
        //            {
        //                Texture tex(InsideModelFolder, TexturePath, aiTextureType_DIFFUSE);
        //                std::unique_ptr<Texture> ptr(new Texture(tex));
        //                ptr->Load(false);
        //                engine->AssimpManager.InsertTextures(FolderName, std::move(ptr), Index);

        //                std::unique_ptr<Texture> ptr2(new Texture(tex));
        //                engine->AssimpManager.InsertTextures(FolderName, std::move(ptr2), Index);
        //            }
        //        }
        //    }
        //}

        ENGINE_CORE_INFO("All Assimp Models Loaded Once");
    }

    std::string AssimpModelManager::GetKey(const std::string& in)
    {
        return AssimpLoadedModels[in]->GetName();
    }

    void AssimpModelManager::MeshDraw(MeshComponent& ModelMesh, unsigned int ID, unsigned int FrameBufferID, FrameBuffer::RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(_camType));

        glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
        auto shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];
        shdrpgm.Use();

        // Check Main Uniforms For each Model
        // Translation done here for each model
        CheckUniformLoc(shdrpgm, _camera, FrameBufferID, ID, box);

        if (_renderMode == FrameBuffer::RenderMode::Fill_Mode)
        {
            // Render
            Render(shdrpgm, GL_FILL, FrameBufferID, ModelMesh, ID, _camType);
        }
        else
        {
            Render(shdrpgm, GL_LINE, FrameBufferID, ModelMesh, ID, _camType);
        }

    }

    void AssimpModelManager::CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int FrameBufferID, unsigned int ModelID, AABB_* box)
    {
        if (engine->world.CheckComponent<MaterialComponent>(ModelID))
        {
            MaterialComponent& material = engine->world.GetComponent<MaterialComponent>(ModelID);
            GLint uniform_var_loc1 = _shdrpgm.GetLocation("material.shininess");
            GLint uniform_var_loc2 = _shdrpgm.GetLocation("material.MaximumShininess");

            GLCall(glUniform1f(uniform_var_loc1, material.shininess));
            GLCall(glUniform1f(uniform_var_loc2, material.MaximumShininess));
        }

        GLint uModelToNDC_ = _shdrpgm.GetLocation("uModelToNDC");
        GLuint model_ = _shdrpgm.GetLocation("model");
        GLuint NoTexures = _shdrpgm.GetLocation("noTex");

        auto& Transform = engine->world.GetComponent<TransformComponent>(ModelID);

        glm::mat4 mModelNDC;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, Transform.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(Transform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Transform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Transform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, Transform.scale.ConvertToGlmVec3Type());
        mModelNDC = _camera.projMtx * _camera.viewMtx * model;
        glUniformMatrix4fv(uModelToNDC_, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(NoTexures, 0);
    }

    unsigned int AssimpModelManager::MeshFactoryCount()
    {
        return AssimpLoadedModels.size();
    }

    void AssimpModelManager::SetTexturesForModel(MaterialComponent& in, std::string& passkey)
    {
        in.TextureKey = passkey;

        // See if can find texture or not
        if (Graphics::textures.find(passkey) != Graphics::textures.end())
        {
            for (int i = 0; i < 2; i++)
            {
                in.HoldingTextures[i].push_back(Graphics::textures[passkey]);
            }
        }
    }

    void AssimpModelManager::InsertTextures(std::string& NameofModel, std::unique_ptr<Texture> in, unsigned int MeshId)
    {
        LoadedTexturesV2[NameofModel][MeshId].push_back(std::move(in));
    }

    void AssimpModelManager::PrintLoadedModels()
    {
        PrintOutModelsLoadedOnce();
        PrintOutModelTextureMap();
        PrintOutModelMap();
        PrintOutAllTextures();
        PrintOutAllMeshes();
    }

    void AssimpModelManager::Cleanup(MeshComponent& in)
    {
        //for (unsigned int i = 0; i < in.Meshes.size(); i++)
        //{
        //    in.Meshes[i].Cleanup();
        //}
    }

    AssimpModelManager::~AssimpModelManager()
    {
        //for (auto i : AssimpModelContainerV2)
        //{
        //    if (i.second != nullptr)
        //    {
        //        delete i.second;
        //    }
        //}
    }

    void AssimpModelManager::InsertModel(unsigned int id)
    {
        // Should already have the model name as key.
        auto& sprite = engine->world.GetComponent<MeshComponent>(id);

        // Assign ModelInfoComponent
        auto& ModelInformation = engine->world.GetComponent<ModeLInforComponent>(id);
        //ModelInformation.NameOfModel = sprite.Key;
        //ModelInformation.Directory = ModelMap[sprite.Key];

        //sprite.Meshes = engine->AssimpManager.AssimpLoadedModels[sprite.Key]->GetMesh();
        //sprite.Textures_loaded = engine->AssimpManager.AssimpLoadedModels[sprite.Key]->GetTextures();

        //for (int i = 0; i < sprite.Meshes.size(); i++)
        //{
        //    auto MeshID = engine->editorManager->CreateDefaultEntity(EntityType::ENT_UNASSIGNED);
        //    sprite.Meshes[i].SetID(MeshID);
        //}

        // If got TextureComponent
        if (engine->world.CheckComponent<TextureComponent>(id))
        {
            auto& tex = engine->world.GetComponent<TextureComponent>(id);
            //engine->AssimpManager.SetTexturesForModel(tex, sprite.Key);
        }
    }

    void AssimpModelManager::TestPath(std::string& path)
    {
        std::ifstream test(path);

        if (!test)
        {
            std::string Error = ("The file path " + path + " doesnt exist! ").c_str();
            ENGINE_LOG_ASSERT(false, Error);
            std::exit(EXIT_FAILURE);
        }
    }

    void AssimpModelManager::Render(Shader& shader, GLenum MOde, unsigned int FrameBufferID, MeshComponent& in, unsigned int ModelID, CameraComponent::CameraType _camType)
    {
        // Each Mesh Render
        Render(shader, MOde, ModelID, in);
    }

    void AssimpModelManager::InsertModelMap(std::string& NameofModel, std::string& Directory)
    {
        ModelMap.insert({ NameofModel,Directory });
    }

    void AssimpModelManager::PrintOutModelsLoadedOnce()
    {
        // Check How Many Models are Loaded
        std::cout << std::endl;
        std::cout << "Loaded Models Count " << AssimpLoadedModels.size() << std::endl;
        std::cout << "-------------------------------------------------------------------" << std::endl;
        for (auto const& Models : AssimpLoadedModels)
        {
            auto& InvidualModels = *(Models.second);
            std::cout << " Model Name : " << InvidualModels.GetName() << std::endl;
            std::cout << " Model Directory : " << InvidualModels.GetDirectory() << std::endl;
            std::cout << " Number of Textures : " << InvidualModels.GetNumberOfTextures() << std::endl;
            std::cout << " Number of Meshes : " << InvidualModels.GetMesh().size() << std::endl;
            InvidualModels.GetTextureNames();
            std::cout << std::endl;
        }
        std::cout << "-------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    }

    void AssimpModelManager::PrintOutModelTextureMap()
    {
        // Check How Many Textures are Loaded and which meshes are they mapped to?
        std::cout << "Loaded Textures Count " << LoadedTexturesV2.size() << std::endl;
        std::cout << "-------------------------------------------------------------------" << std::endl;
        for (auto const& Model : LoadedTexturesV2)
        {
            auto& ModelName = (Model.first);
            auto& MapWithMeshIndexAndTextures = (Model.second);

            std::cout << "Model Name : " << ModelName << std::endl;

            for (auto& i : MapWithMeshIndexAndTextures)
            {
                for (auto& EachTextures : i.second)
                {
                    std::cout << "MeshIndex [" << i.first << "] " << EachTextures->GetPath() << std::endl;
                }
            }
            std::cout << std::endl;
        }
        std::cout << "-------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    }

    void AssimpModelManager::PrintOutModelMap()
    {
        // ModelInformation loaded
        std::cout << "Loaded ModelMap Count " << ModelMap.size() << std::endl;
        std::cout << "-------------------------------------------------------------------" << std::endl;
        for (auto const& Model : ModelMap)
        {
            auto& ModelName = (Model.first); // Folder Name too
            auto& ModelPath = (Model.second);

            std::cout << "Model Name : " << ModelName << " ==== " << ModelPath << std::endl;
        }
        std::cout << "-------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    }

    void AssimpModelManager::PrintOutAllTextures()
    {
        std::cout << "Loaded Textures Count " << Graphics::textures.size() << std::endl;
        std::cout << "-------------------------------------------------------------------" << std::endl;
        for (const auto& i : Graphics::textures)
        {
            auto& TextureName = i.first;
            auto& TextureItself = i.second;

            std::cout << "Texture Name " << TextureName << std::endl;
        }
        std::cout << "-------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    }

    void AssimpModelManager::PrintOutAllMeshes()
    {
        std::cout << "loaded Meshes Count " << SingleMeshMap.size() << std::endl;
        std::cout << "-------------------------------------------------------------------" << std::endl;
        for (const auto& i : SingleMeshMap)
        {
            auto& MeshName = i.first;
            auto& Mesh = i.second;

            std::cout << "Mesh Name " << MeshName << std::endl;
            std::cout << "Mesh Vertices " << Mesh->GetVertices().size() << std::endl;
            std::cout << std::endl;
        }
        std::cout << "-------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    }

    void AssimpModelManager::SetMeshComponent(unsigned int ID, std::string in)
    {
        if (engine->world.CheckComponent<MeshComponent>(ID))
        {
            auto& Mesh = engine->world.GetComponent<MeshComponent>(ID);

            Mesh.MeshName = SingleMeshMap[in]->MeshName;
            Mesh.VBO = SingleMeshMap[in]->VBO;
            Mesh.VAO = SingleMeshMap[in]->VAO;
            Mesh.EBO = SingleMeshMap[in]->EBO;
            Mesh.NoTex = SingleMeshMap[in]->NoTex;
            Mesh.Diffuse = SingleMeshMap[in]->Diffuse;
            Mesh.Specular = SingleMeshMap[in]->Specular;
            Mesh.Ambient = SingleMeshMap[in]->Ambient;
            Mesh.Vertices = SingleMeshMap[in]->Vertices;
            Mesh.Indices = SingleMeshMap[in]->Indices;
        }
    }

    void AssimpModelManager::Render(Shader& shader, GLenum mode, unsigned int id, MeshComponent& in)
    {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, mode);

        // If dont have textures ( Flagged as True )
        if (in.NoTex && (!engine->world.CheckComponent<TextureComponent>(id)))
        {
            GLint uniform_var_loc1 = shader.GetLocation("BasicPrimitives");
            GLint uniform_var_loc2 = shader.GetLocation("uColor");
            GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
            GLuint tex_loc = shader.GetLocation("uTex2d");
            GLuint diff0 = shader.GetLocation("sdiffuse");
            GLuint spec = shader.GetLocation("sspecular");
            GLuint Texture = shader.GetLocation("noTex");

            glUniform1i(uniform_var_loc3, true);
            glUniform4f(diff0, in.Diffuse.r, in.Diffuse.g, in.Diffuse.b, in.Diffuse.a);
            glUniform4f(spec, in.Specular.r, in.Specular.g, in.Specular.b, in.Specular.a);
            glUniform1i(Texture, true);
            glUniform1i(tex_loc, false);
            glUniform1i(uniform_var_loc1, false);
        }
        else
        {
            if (engine->world.CheckComponent<MaterialComponent>(id))
            {
                MaterialComponent& tex = engine->world.GetComponent<MaterialComponent>(id);

                // textures
                unsigned int diffuseIdx = 0;
                unsigned int specularIdx = 0;

                for (unsigned int i = 0; i < tex.HoldingTextures[i].size(); i++)
                {
                    // activate texture
                    glActiveTexture(GL_TEXTURE0 + i);

                    // retrieve texture info
                    std::string name;
                    switch (tex.HoldingTextures[i][i].GetType())
                    {
                    case aiTextureType_DIFFUSE:
                        name = "diffuse" + std::to_string(diffuseIdx++);
                        break;
                    case aiTextureType_SPECULAR:
                        name = "specular" + std::to_string(specularIdx++);
                        break;
                    }

                    GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
                    GLuint diff0 = shader.GetLocation("diffuse0");
                    GLuint spec = shader.GetLocation("specular0");
                    GLuint dsa = shader.GetLocation("noTex");

                    glUniform1i(uniform_var_loc3, true);
                    glUniform1i(diff0, i);
                    glUniform1i(spec, i);
                    glUniform1i(dsa, false);

                    // bind texture
                    tex.HoldingTextures[i][i].Bind();
                }

                // If no Textures , We leave it blank until it has textures
                if (tex.HoldingTextures[0].size() == 0)
                {
                    GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
                    glUniform1i(uniform_var_loc3, false);
                }
            }
            else
            {
                GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
                glUniform1i(uniform_var_loc3, false);
            }
        }

        // reset
        glActiveTexture(GL_TEXTURE0);

        // EBO stuff
        glBindVertexArray(in.VAO);
        glDrawElements(GL_TRIANGLES, in.Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void AssimpModelManager::Render(GLenum mode, MeshComponent& in)
    {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, mode);

        // EBO stuff
        glBindVertexArray(in.VAO);
        glDrawElements(GL_TRIANGLES, in.Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void AssimpModelManager::SetSingleMesh(unsigned int ID, std::string& MeshName)
    {
        auto& Mesh = engine->world.GetComponent<MeshComponent>(ID);
        engine->AssimpManager.SetMeshComponent(ID, MeshName);

        // If got TextureComponent
        if (engine->world.CheckComponent<MaterialComponent>(ID))
        {
            auto& tex = engine->world.GetComponent<MaterialComponent>(ID);
            engine->AssimpManager.SetTexturesForModel(tex, Mesh.MeshName);
        }

    }

}

namespace Eclipse
{
    void AssimpModelManager::Draw(unsigned int FrameBufferID, FrameBuffer::RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(_camType));

        glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
        auto shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];

        shdrpgm.Use();

        for (auto const& Models : AssimpModelContainer_)
        {
            auto& ID = Models.first;
            auto& InvidualModels = *(Models.second);

            engine->MaterialManager.UpdateStencilWithActualObject(ID);

            // Check Main Uniforms For each Model
            // Translation done here for each model
            CheckUniformLoc(shdrpgm, _camera, FrameBufferID, ID, box);

            if (_renderMode == FrameBuffer::RenderMode::Fill_Mode)
            {
                // Render
                InvidualModels.Render(shdrpgm, GL_FILL, FrameBufferID, ID);
            }
            else
            {
                InvidualModels.Render(shdrpgm, GL_LINE, FrameBufferID, ID);
            }

        }

        shdrpgm.UnUse();
    }

    AssimpModel* AssimpModelManager::GetModel(unsigned int ID)
    {
        for (auto const& it : AssimpModelContainer_)
        {
            auto& ID = (it.first);
            auto& SelectedModel = *(it.second);

            if (ID == ID)
            {
                return &SelectedModel;
            }
        }

        return nullptr;
    }

    bool AssimpModelManager::ClearContainer()
    {
        AssimpModelContainer_.clear();

        return true;
    }

    void AssimpModelManager::CreateModel(unsigned int ID, std::string name, std::string FolderName, std::string filename)
    {
        // Add Required Components
        engine->world.AddComponent(ID, MaterialComponent{});

        TransformComponent& Transform = engine->world.GetComponent<TransformComponent>(ID);
        Transform.scale.setX(5);
        Transform.scale.setY(5);
        Transform.scale.setZ(5);

        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialModelType::MT_MODELS3D;
        // ----------------------------------------------------------------------------------------------------------

        // Create path
        std::string PathName = ("src/Assets/ASSModels/" + FolderName + "/" + filename).c_str();

        // Test Path
        TestPath(PathName);

        // Always set False because we have textures
        AssimpModel* NewModel = new AssimpModel(false);
        NewModel->LoadAssimpModel(PathName);
        NewModel->SetProperties(name, ModelType::MT_ANIMAL, ID);

        // ----------------------------------------------------------------------------------------------------------

        //// Insert
        if (AssimpModelContainer_.insert(std::pair<unsigned int, AssimpModel*>(ID, NewModel)).second == true)
        {
            std::string Success = ("3D Model [" + name + "] Created and Inseted into Container Successfully! ").c_str();
            ENGINE_CORE_INFO(Success);
        }

        // ----------------------------------------------------------------------------------------------------------
        engine->MaterialManager.RegisterMeshForHighlighting(ID);
        // ----------------------------------------------------------------------------------------------------------
    }

    void AssimpModelManager::HighlihtDraw(unsigned int FrameBufferID, GLenum Mode)
    {
        if (0) // if highlight is true
        {
            auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

            glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
            auto shdrpgm = Graphics::shaderpgms.find("OutLineShader");

            shdrpgm->second.Use();

            for (auto const& Models : AssimpModelContainer_)
            {
                auto& ID = Models.first;
                auto& InvidualModels = *(Models.second);

                // Check Main Uniforms For each Model
                // Translation done here for each model
                //CheckUniformLoc(shdrpgm->second, _camera, FrameBufferID, ID);

                GLuint uniformloc1 = shdrpgm->second.GetLocation("outlining");
                GLCall(glUniform1f(uniformloc1, 0.05f));

                // Render
                InvidualModels.Render(shdrpgm->second, Mode, FrameBufferID, ID);
            }

            shdrpgm->second.UnUse();
        }
    }

    AssimpModelContainer AssimpModelManager::GetContainer()
    {
        return AssimpModelContainer_;
    }

    unsigned int AssimpModelManager::AssimpModelCount()
    {
        return AssimpModelContainer_.size();
    }

    void AssimpModelManager::DeleteItem(unsigned int index, AssimpModel* model_ptr)
    {
        for (auto AssimpIT = AssimpModelContainer_.begin(); AssimpIT != std::end(AssimpModelContainer_); ++AssimpIT)
        {
            if (((*AssimpIT).first == index) && ((*AssimpIT).second == model_ptr))
            {
                AssimpModelContainer_.erase(AssimpIT);

                std::string Name = (*AssimpIT).second->GetName();
                std::string SuccessMsg = ("Model [ " + Name + " ] + erased from AssimpModelContainer_").c_str();
                ENGINE_CORE_INFO(SuccessMsg);
                return;
            }
        }
    }

    void AssimpModelManager::CleanUpAllModels()
    {
        for (auto const& Models : AssimpModelContainer_)
        {
            auto& InvidualModels = *(Models.second);
            InvidualModels.Cleanup();
        }
    }


    bool AssimpModelManager::InsertModel(AssimpModel& in)
    {
        // Insert
        if (AssimpModelContainer_.insert({ in.GetEntityID() , &in }).second == true)
        {
            return true;
        }

        return false;
    }

    MeshModelContainer AssimpModelManager::GetMeshContainer()
    {
        return AssimpModelContainerV2;
    }

    unsigned int AssimpModelManager::MeshModelCount()
    {
        return AssimpModelContainerV2.size();
    }

    void AssimpModelManager::DeleteItem(unsigned int index)
    {
        AssimpMeshIT it = AssimpModelContainerV2.find(index);
        AssimpModelContainerV2.erase(index);
    }

    bool AssimpModelManager::InsertMesh(MeshComponent& in)
    {
        //// Insert
        //if (AssimpModelContainerV2.insert({ in.ID , &in }).second == true)
        //{
        //	return true;
        //}

        return false;
    }

    void AssimpModelManager::CleanUpAllModelsMeshes()
    {
        for (auto const& Models : AssimpModelContainerV2)
        {
            auto& InvidualModels = *(Models.second);
            //Cleanup(InvidualModels);
        }
    }
}
