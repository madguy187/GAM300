#include "pch.h"
#include "AssimpManager.h"

namespace Eclipse
{
    typedef std::multimap<std::string, Texture>::iterator MMAPIterator;

    bool AssimpModelManager::GeometryContainerCheck(const std::string& in)
    {
        if (Geometry.find(in) != engine->AssimpManager.Geometry.end())
        {
            return true;
        }

        return false;
    }

    void AssimpModelManager::InsertTextures(std::string& NameofModel, std::unique_ptr<Texture> in, unsigned int MeshId)
    {
        LoadedTexturesV2[NameofModel][MeshId].push_back(std::move(in));
    }

    void AssimpModelManager::ClearGeometry()
    {
        Geometry.clear();
    }

    void AssimpModelManager::ClearAllMeshNames()
    {
        AllMeshNames.clear();
    }

    void AssimpModelManager::ClearAllPrefabs()
    {
        Prefabs.clear();
    }

    void AssimpModelManager::CreateModel(unsigned int ID, const std::string& ModelName)
    {
        (void)ID;

        if (engine->gEngineCompiler->AreAllCompiled())
        {
            if (Prefabs.find(ModelName) == Prefabs.end())
            {
                auto MeshID = engine->editorManager->CreateDefaultEntity(EntityType::ENT_MODEL);
                // Cannot Find this as a parent
                std::string Name = ModelName;
                engine->world.AddComponent(MeshID, MeshComponent{});
                engine->world.AddComponent(MeshID, ModelComponent{});
                engine->world.AddComponent(MeshID, MaterialComponent{ MaterialModelType::MT_MODELS3D });
                SetSingleMesh(MeshID, Name);
            }
            else
            {
                if (Prefabs[ModelName].size() == 0)
                {
                    ENGINE_LOG_ASSERT(false, "Cannot Find Model");
                    return;
                }

                // Is a prefab since its a parent
                std::string NameOfFolder = ModelName;
                for (int i = 0; i < Prefabs[NameOfFolder].size(); i++)
                {
                    auto& name = Prefabs[NameOfFolder][i];
                    auto MeshID = engine->editorManager->CreateDefaultEntity(EntityType::ENT_MODEL);

                    engine->world.AddComponent(MeshID, MeshComponent{});
                    engine->world.AddComponent(MeshID, ModelComponent{});
                    engine->world.AddComponent(MeshID, MaterialComponent{ MaterialModelType::MT_MODELS3D });
                    SetSingleMesh(MeshID, name);
                }
            }
        }
    }

    void AssimpModelManager::MeshDraw(MeshComponent& ModelMesh, unsigned int ID, FrameBufferMode Mode, RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(_camType));
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(_camType));

        engine->gFrameBufferManager->UseFrameBuffer(Mode);

        auto shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];
        shdrpgm.Use();

        if (engine->GraphicsManager.EnableEnvironmentMapForAll && engine->GraphicsManager.DrawSky)
            ModelMesh.ENV_MAP = true;
        else
            ModelMesh.ENV_MAP = false;

        if (ModelMesh.ENV_MAP == true && engine->GraphicsManager.DrawSky == true)
        {
            if (engine->gFrameBufferManager->CheckFrameBuffer(engine->gFrameBufferManager->GetFrameBufferID(Mode), FrameBufferMode::FBM_SCENE))
            {
                //engine->MaterialManager.DoNotUpdateStencil();
                shdrpgm = Graphics::shaderpgms["EnvironmentMap"];
                shdrpgm.Use();
                GLuint view = shdrpgm.GetLocation("view");
                GLuint cameraPos = shdrpgm.GetLocation("cameraPos");
                GLint projection = shdrpgm.GetLocation("projection");
                GLint TypeOfMap = shdrpgm.GetLocation("Type");

                glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
                glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
                GLCall(glUniform3f(cameraPos, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ()));
                glUniform1i(TypeOfMap, static_cast<GLint>(ModelMesh.ENV_TYPE));
            }
        }

        CheckUniformLoc(shdrpgm, _camera, ID, box);

        if (_renderMode == RenderMode::Fill_Mode)
        {
            Render(shdrpgm, GL_FILL, ModelMesh, ID);
        }
        else
        {
            Render(shdrpgm, GL_LINE, ModelMesh, ID);
        }
    }

    void AssimpModelManager::DebugNormals(MeshComponent& ModelMesh, unsigned int ID, FrameBufferMode In, CameraComponent::CameraType _camType)
    {
        if (engine->GraphicsManager.VisualizeNormalVectors == true)
        {
            auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(_camType));

            engine->gFrameBufferManager->UseFrameBuffer(In);

            auto shdrpgm = Graphics::shaderpgms["DebugNormals"];
            shdrpgm.Use();

            GLuint view = shdrpgm.GetLocation("view");
            GLint projection = shdrpgm.GetLocation("projection");
            GLuint model_ = shdrpgm.GetLocation("model");
            GLint uniform_var_loc5 = shdrpgm.GetLocation("MAGNITUDE");

            auto& Transform = engine->world.GetComponent<TransformComponent>(ID);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, Transform.position.ConvertToGlmVec3Type());
            model = glm::rotate(model, glm::radians(Transform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(Transform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(Transform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, Transform.scale.ConvertToGlmVec3Type());
            glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
            glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
            GLCall(glUniform1f(uniform_var_loc5, engine->GraphicsManager.Magnitude));


            // EBO stuff
            glBindVertexArray(engine->AssimpManager.Geometry[ModelMesh.MeshName.data()]->VAO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(engine->AssimpManager.Geometry[ModelMesh.MeshName.data()]->Indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    std::unordered_map<std::string, std::vector<std::string>>& AssimpModelManager::GetPrefabs()
    {
        return Prefabs;
    }

    void AssimpModelManager::InsertPrimitiveName(const std::string& in)
    {
        AllPrimitiveModelsNames.push_back(in);
    }

    void AssimpModelManager::InsertMeshName(const std::string& in)
    {
        AllMeshNames.push_back(in);
    }

    void AssimpModelManager::InsertGeometry(const std::string& name, Mesh& NewMesh)
    {
        Geometry.emplace(name, std::make_unique<Mesh>(NewMesh));
    }

    void AssimpModelManager::InsertPrefabs(const std::string& Index_, const std::string& MeshName_)
    {
        Prefabs[Index_].push_back(MeshName_.data());
    }

    void AssimpModelManager::InsertGeometryName(const std::string& MeshName_)
    {
        AllGeometryNames.push_back(MeshName_);
    }

    std::vector<std::string>& AssimpModelManager::GetMeshNames()
    {
        return AllMeshNames;
    }

    std::vector<std::string>& AssimpModelManager::GetPrimitiveNames()
    {
        return AllPrimitiveModelsNames;
    }

    void AssimpModelManager::CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int ModelID, AABB_* box)
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

        BoundingRegion br(Transform.position.ConvertToGlmVec3Type(), Transform.scale.ConvertToGlmVec3Type());
        box->AddInstance(br);
    }

    void AssimpModelManager::SetTexturesForModel(MaterialComponent& in, std::string& passkey)
    {
        in.TextureKey = passkey;

        std::pair<MMAPIterator, MMAPIterator> result = Graphics::textures.equal_range(passkey);

        for (MMAPIterator it = result.first; it != result.second; it++)
        {
            in.HoldingTextures.push_back(it->second);
        }
    }

    void AssimpModelManager::Cleanup(MeshComponent& in)
    {
        (void)in;

        //for (unsigned int i = 0; i < in.Meshes.size(); i++)
        //{
        //    in.Meshes[i].Cleanup();
        //}
    }

    AssimpModelManager::~AssimpModelManager()
    {
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

    void AssimpModelManager::Render(Shader& shader, GLenum MOde, MeshComponent& in, unsigned int ModelID)
    {
        // Each Mesh Render
        Render(shader, MOde, ModelID, in);
    }

    void AssimpModelManager::PrintOutAllTextures()
    {
        std::cout << "Loaded Textures Count " << Graphics::textures.size() << std::endl;
        std::cout << "-------------------------------------------------------------------" << std::endl;
        for (const auto& i : Graphics::textures)
        {
            auto& TextureName = i.first;

            std::cout << "Texture Name " << TextureName << std::endl;
        }
        std::cout << "-------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    }

    void AssimpModelManager::PrintOutAllMeshes()
    {
        std::cout << "loaded Meshes Count " << Geometry.size() << std::endl;
        std::cout << "-------------------------------------------------------------------" << std::endl;
        for (const auto& i : Geometry)
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

            char* Name = in.data();
            strcpy_s(Mesh.MeshName.data(), Mesh.MeshName.size(), Name);
        }
    }

    void AssimpModelManager::Render(Shader& shader, GLenum mode, unsigned int id, MeshComponent& in)
    {
        engine->BatchRenderer.DynamicVertices.clear();
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        //glDisable(GL_CULL_FACE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);
        glPolygonMode(GL_FRONT_AND_BACK, mode);

        // If dont have textures ( Flagged as True )
        if (engine->AssimpManager.Geometry[in.MeshName.data()]->NoTex && (!engine->world.CheckComponent<TextureComponent>(id)))
        {
            GLint uniform_var_loc1 = shader.GetLocation("BasicPrimitives");
            //GLint uniform_var_loc2 = shader.GetLocation("uColor");
            GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
            GLuint tex_loc = shader.GetLocation("uTex2d");
            GLuint diff0 = shader.GetLocation("sdiffuse");
            GLuint spec = shader.GetLocation("sspecular");
            GLuint Texture = shader.GetLocation("noTex");

            glUniform1i(uniform_var_loc3, true);
            glUniform4f(diff0,
                engine->AssimpManager.Geometry[in.MeshName.data()]->Diffuse.r,
                engine->AssimpManager.Geometry[in.MeshName.data()]->Diffuse.g,
                engine->AssimpManager.Geometry[in.MeshName.data()]->Diffuse.b,
                engine->AssimpManager.Geometry[in.MeshName.data()]->Diffuse.a);
            glUniform4f(spec,
                engine->AssimpManager.Geometry[in.MeshName.data()]->Specular.r,
                engine->AssimpManager.Geometry[in.MeshName.data()]->Specular.g,
                engine->AssimpManager.Geometry[in.MeshName.data()]->Specular.b,
                engine->AssimpManager.Geometry[in.MeshName.data()]->Specular.a);
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
                unsigned int normalIdx = 0;

                for (unsigned int it = 0; it < tex.HoldingTextures.size(); it++)
                {
                    // retrieve texture info
                    std::string name;
                    switch (tex.HoldingTextures[it].GetType())
                    {
                    case aiTextureType_DIFFUSE:
                        // activate texture
                        glActiveTexture(GL_TEXTURE0 + it);
                        name = "diffuse" + std::to_string(diffuseIdx++);
                        break;
                    case aiTextureType_SPECULAR:
                        // activate texture
                        glActiveTexture(GL_TEXTURE0 + it);
                        name = "specular" + std::to_string(specularIdx++);
                        break;
                    case aiTextureType_NORMALS:
                        // activate texture
                        glActiveTexture(GL_TEXTURE0 + it);
                        name = "normal" + std::to_string(normalIdx++);
                        break;
                    }

                    if (tex.HoldingTextures[it].GetType() != aiTextureType_NORMALS)
                    {
                        GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
                        GLuint diff0 = shader.GetLocation("diffuse0");
                        GLuint spec = shader.GetLocation("specular0");
                        GLuint noTex = shader.GetLocation("noTex");
                        GLuint CheckNormapMap = shader.GetLocation("checkNormalMap");

                        glUniform1i(noTex, false);
                        glUniform1i(uniform_var_loc3, true);
                        glUniform1i(CheckNormapMap, false);
                        glUniform1i(diff0, it);
                        glUniform1i(spec, it);
                        tex.HoldingTextures[it].Bind();
                    }
                    else
                    {
                        GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
                        GLuint CheckNoTex = shader.GetLocation("noTex");
                        GLuint CheckNormapMap = shader.GetLocation("checkNormalMap");
                        GLuint normal0 = shader.GetLocation("normal0");
                        glUniform1i(uniform_var_loc3, true);
                        glUniform1i(CheckNoTex, false);

                        if (engine->GraphicsManager.EnableNormalMapping)
                        {
                            glUniform1i(CheckNormapMap, true);
                        }
                        else
                        {
                            glUniform1i(CheckNormapMap, false);
                        }

                        glUniform1i(normal0, it);
                        tex.HoldingTextures[it].Bind();
                    }
                }

                // If no Textures , We leave it blank until it has textures
                if (tex.HoldingTextures.size() == 0)
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

        // dynamic
        glBindBuffer(GL_ARRAY_BUFFER, engine->BatchRenderer.m_QuadVB);

        //engine->BatchRenderer.DynamicVertices.insert(
        //    engine->BatchRenderer.DynamicVertices.end(), // empty one
        //    &engine->AssimpManager.Geometry[in.MeshName.data()]->Vertices[0], // assimp
        //    &engine->AssimpManager.Geometry[in.MeshName.data()]->Vertices[engine->AssimpManager.Geometry[in.MeshName.data()]->Vertices.size()]
        //);

        engine->BatchRenderer.DynamicVertices.assign(
            engine->AssimpManager.Geometry[in.MeshName.data()]->Vertices.begin(),
            engine->AssimpManager.Geometry[in.MeshName.data()]->Vertices.end()
        );

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * (engine->BatchRenderer.DynamicVertices.size()), &engine->BatchRenderer.DynamicVertices[0]);

        // EBO stuff
        glBindVertexArray(engine->BatchRenderer.m_QuadVA);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(engine->AssimpManager.Geometry[in.MeshName.data()]->Indices.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // reset
        glActiveTexture(GL_TEXTURE0);
    }

    void AssimpModelManager::Render(GLenum mode, MeshComponent& in)
    {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, mode);

        // EBO stuff
        glBindVertexArray(engine->AssimpManager.Geometry[in.MeshName.data()]->VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(engine->AssimpManager.Geometry[in.MeshName.data()]->Indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void AssimpModelManager::SetSingleMesh(unsigned int ID, std::string& MeshName_)
    {
        auto& Mesh = engine->world.GetComponent<MeshComponent>(ID);
        engine->AssimpManager.SetMeshComponent(ID, MeshName_);

        // If got TextureComponent
        if (engine->world.CheckComponent<MaterialComponent>(ID))
        {
            auto& tex = engine->world.GetComponent<MaterialComponent>(ID);
            std::string MeshNameh = Mesh.MeshName.data();
            engine->AssimpManager.SetTexturesForModel(tex, MeshNameh);
        }

    }
}

namespace Eclipse
{
    void AssimpModelManager::Draw(unsigned int FrameBufferID, RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType)
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
            CheckUniformLoc(shdrpgm, _camera, ID, box);

            if (_renderMode == RenderMode::Fill_Mode)
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

    }

    void AssimpModelManager::HighlihtDraw(unsigned int FrameBufferID, GLenum Mode)
    {
        if (0) // if highlight is true
        {
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

    size_t AssimpModelManager::AssimpModelCount()
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

    size_t AssimpModelManager::MeshModelCount()
    {
        return AssimpModelContainerV2.size();
    }

    void AssimpModelManager::DeleteItem(unsigned int index)
    {
        AssimpMeshIT it = AssimpModelContainerV2.find(index);
        AssimpModelContainerV2.erase(index);
    }

    std::string AssimpModelManager::GetKey(const std::string& in)
    {
        return AssimpLoadedModels[in]->GetName();
    }

    bool AssimpModelManager::CheckGeometryExist(MeshComponent& in)
    {
        if (Geometry.find(in.MeshName.data()) == Geometry.end())
            return false;

        return true;
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

        // ModelInformation loaded
        std::cout << "All Meshes Count " << AllMeshNames.size() << std::endl;
        std::cout << "-------------------------------------------------------------------" << std::endl;
        for (auto const& Model : AllMeshNames)
        {
            std::cout << "Model Name : " << Model << std::endl;
        }
        std::cout << "-------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    }

    size_t AssimpModelManager::MeshFactoryCount()
    {
        return AssimpLoadedModels.size();
    }

}
