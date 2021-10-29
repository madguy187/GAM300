#include "pch.h"
#include "AssimpManager.h"

#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"

namespace Eclipse
{
    void AssimpModelManager::MeshEditorUniforms(Shader& _shdrpgm, CameraComponent& _camera, unsigned int ModelID)
    {
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(_camera.camType));

        GLuint view = _shdrpgm.GetLocation("view");
        GLuint cameraPos = _shdrpgm.GetLocation("camPos");
        GLint projection = _shdrpgm.GetLocation("projection");
        GLuint model_ = _shdrpgm.GetLocation("model");

        auto& Transform = engine->world.GetComponent<TransformComponent>(ModelID);

        glm::mat4 mModelNDC;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, Transform.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(Transform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Transform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Transform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, Transform.scale.ConvertToGlmVec3Type());
        mModelNDC = _camera.projMtx * _camera.viewMtx * model;
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
        GLCall(glUniform3f(cameraPos, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ()));

    }

    void AssimpModelManager::MeshEditorDraw(World& world_, MeshComponent& ModelMesh, unsigned int ID, FrameBufferMode in, CameraComponent::CameraType _camType)
    {
        auto& _camera = world_.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(_camType));
        engine->gFrameBufferManager->UseFrameBuffer(in);

        auto shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        MeshEditorUniforms(shdrpgm, _camera, ID);
        CheckUniforms(shdrpgm, ID, ModelMesh, _camera);

        RenderMesh(ModelMesh, GL_FILL);
    }

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

    void AssimpModelManager::CreateBasicPrimitives(Entity ID, const std::string& ModelName)
    {
        auto& name = Prefabs[ModelName][0];
        engine->world.AddComponent(ID, MeshComponent{});
        engine->world.AddComponent(ID, ModelComponent{});
        engine->world.AddComponent(ID, MaterialComponent{ MaterialModelType::MT_MODELS3D });
        SetSingleMesh(ID, name);
    }

    Entity AssimpModelManager::CreateModel(unsigned int ID, const std::string& ModelName)
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

                return MeshID;
            }
            else
            {
                if (Prefabs[ModelName].size() == 0)
                {
                    ENGINE_LOG_ASSERT(false, "Cannot Find Model");
                    return MAX_ENTITY;
                }
                else if (Prefabs[ModelName].size() == 1)
                {
                    auto& name = Prefabs[ModelName][0];
                    Entity MeshID = engine->editorManager->CreateDefaultEntity(EntityType::ENT_MODEL);
                    engine->world.AddComponent(MeshID, MeshComponent{});
                    engine->world.AddComponent(MeshID, ModelComponent{});
                    engine->world.AddComponent(MeshID, MaterialComponent{ MaterialModelType::MT_MODELS3D });
                    SetSingleMesh(MeshID, name);
                    return MeshID;
                }
                else
                {
                    // Is a prefab since its a parent
                    std::string NameOfFolder = ModelName;
                    Entity ParentID = 0;
                    Entity MeshID = 0;
                    ParentID = engine->editorManager->CreateDefaultEntity(EntityType::ENT_MODEL);
                    engine->world.AddComponent(ParentID, ParentComponent{});
                    TransformComponent& parentTrans = engine->world.GetComponent<TransformComponent>(MeshID);

                    for (int i = 0; i < Prefabs[NameOfFolder].size(); i++)
                    {
                        auto& name = Prefabs[NameOfFolder][i];
                        MeshID = engine->editorManager->CreateDefaultEntity(EntityType::ENT_MESH);
                        EntityComponent* test = &engine->world.GetComponent<EntityComponent>(ParentID);
                        EntityComponent* Child = &engine->world.GetComponent<EntityComponent>(MeshID);

                        engine->world.AddComponent(MeshID, ChildComponent{});

                        test->Child.push_back(MeshID);
                        Child->IsAChild = true;
                        Child->Parent.push_back(ParentID);

                        engine->world.GetComponent<ParentComponent>(ParentID).child.push_back(MeshID);

                        ChildComponent& childComp = engine->world.GetComponent<ChildComponent>(MeshID);
                        TransformComponent& childTrans = engine->world.GetComponent<TransformComponent>(MeshID);

                        childComp.parentIndex = ParentID;
                        childComp.ScaleOffset.setX(childTrans.scale.getX() / parentTrans.scale.getX());
                        childComp.ScaleOffset.setY(childTrans.scale.getY() / parentTrans.scale.getY());
                        childComp.ScaleOffset.setZ(childTrans.scale.getZ() / parentTrans.scale.getZ());
                        engine->world.AddComponent(MeshID, MeshComponent{});
                        engine->world.AddComponent(MeshID, ModelComponent{});
                        engine->world.AddComponent(MeshID, MaterialComponent{ MaterialModelType::MT_MODELS3D });
                        SetSingleMesh(MeshID, name);
                    }

                    return MeshID;
                }
            }
        }

        return MAX_ENTITY;
    }


    void AssimpModelManager::RegisterExistingModel(Entity ID, const std::string& ModelName)
    {
        if (engine->gEngineCompiler->AreAllCompiled())
        {
            if (ModelName.empty()) return;

            if (Prefabs.find(ModelName) == Prefabs.end())
            {
                std::string Name = ModelName;
                SetSingleMesh(ID, Name);
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
                    SetSingleMesh(ID, name);
                }
            }
        }
    }

    void AssimpModelManager::MeshDraw(MeshComponent& ModelMesh, unsigned int ID, FrameBufferMode Mode, RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(_camType));
        engine->gFrameBufferManager->UseFrameBuffer(Mode);

        auto shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        gEnvironmentMap.CheckUniform(ModelMesh, _camera);
        ChecModelkUniforms(shdrpgm, _camera, ID, box);
        CheckUniforms(shdrpgm, ID, ModelMesh, _camera);

        if (_renderMode == RenderMode::Fill_Mode)
        {
            RenderMesh(ModelMesh, GL_FILL);
        }
        else
        {
            RenderMesh(ModelMesh, GL_LINE);
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
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(_camera.camType));

        if (engine->world.CheckComponent<MaterialComponent>(ModelID))
        {
            MaterialComponent& material = engine->world.GetComponent<MaterialComponent>(ModelID);
            GLint uniform_var_loc1 = _shdrpgm.GetLocation("material.shininess");
            GLint uniform_var_loc2 = _shdrpgm.GetLocation("material.MaximumShininess");

            GLCall(glUniform1f(uniform_var_loc1, material.shininess));
            GLCall(glUniform1f(uniform_var_loc2, material.MaximumShininess));
        }

        GLuint view = _shdrpgm.GetLocation("view");
        GLuint cameraPos = _shdrpgm.GetLocation("camPos");
        GLint projection = _shdrpgm.GetLocation("projection");

        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
        GLCall(glUniform3f(cameraPos, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ()));


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
        (void)mode;
        //glEnable(GL_BLEND);
        //glEnable(GL_DEPTH_TEST);
        ////glDisable(GL_CULL_FACE);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);
        //glFrontFace(GL_CCW);
        //glPolygonMode(GL_FRONT_AND_BACK, mode);

        // If dont have textures ( Flagged as True )
        if (engine->AssimpManager.Geometry[in.MeshName.data()]->NoTex && (!engine->world.CheckComponent<TextureComponent>(id)))
        {
            GLint uniform_var_loc1 = shader.GetLocation("BasicPrimitives");
            //GLint uniform_var_loc2 = shader.GetLocation("uColor");
            GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
            GLuint tex_loc = shader.GetLocation("uTex2d");
            GLuint diff0 = shader.GetLocation("AlbedoConstant");
            //GLuint spec = shader.GetLocation("sspecular");
            GLuint Texture = shader.GetLocation("noTex");
            GLuint HasTexture = shader.GetLocation("HasTexture");
            GLuint MetallicConstant = shader.GetLocation("MetallicConstant");
            GLuint RoughnessConstant = shader.GetLocation("RoughnessConstant");
            GLuint AoConstant = shader.GetLocation("AoConstant");

            GLCall(glUniform1f(AoConstant, 1.0f));
            GLCall(glUniform1f(MetallicConstant, 0.5f));
            GLCall(glUniform1f(RoughnessConstant, 0.5f));
            GLCall(glUniform1i(HasTexture, false));
            glUniform1i(uniform_var_loc3, true);

            //GLCall(glUniform3f(diff0, 0.7, 0.003, 0.6));
            glUniform3f(diff0,
                engine->AssimpManager.Geometry[in.MeshName.data()]->Diffuse.r,
                engine->AssimpManager.Geometry[in.MeshName.data()]->Diffuse.g,
                engine->AssimpManager.Geometry[in.MeshName.data()]->Diffuse.b);

            // glUniform4f(spec,
            //     engine->AssimpManager.Geometry[in.MeshName.data()]->Specular.r,
            //     engine->AssimpManager.Geometry[in.MeshName.data()]->Specular.g,
            //     engine->AssimpManager.Geometry[in.MeshName.data()]->Specular.b,
            //     engine->AssimpManager.Geometry[in.MeshName.data()]->Specular.a);
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

                    GLuint metallic = shader.GetLocation("metallic");
                    GLuint roughness = shader.GetLocation("roughness");
                    GLuint HasTexture = shader.GetLocation("HasTexture");
                    glUniform1i(HasTexture, true);
                    GLCall(glUniform1f(metallic, 1.0f));
                    GLCall(glUniform1f(roughness, 1.0f));

                    if (tex.HoldingTextures[it].GetType() != aiTextureType_NORMALS)
                    {
                        GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
                        GLuint diff0 = shader.GetLocation("albedoMap");
                        GLuint spec = shader.GetLocation("albedoMap");
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
                        GLuint normal0 = shader.GetLocation("normalMap");
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

        // reset
        glActiveTexture(GL_TEXTURE0);

        //// EBO stuff
        //glBindVertexArray(engine->AssimpManager.Geometry[in.MeshName.data()]->VAO);
        //glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(engine->AssimpManager.Geometry[in.MeshName.data()]->Indices.size()), GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);
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

    void AssimpModelManager::RenderMesh(MeshComponent& In, GLenum Mode)
    {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        //glDisable(GL_CULL_FACE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);
        glPolygonMode(GL_FRONT_AND_BACK, Mode);

        glBindVertexArray(Geometry[In.MeshName.data()]->VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Geometry[In.MeshName.data()]->Indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void AssimpModelManager::CheckUniforms(Shader& shader, Entity EntityID, MeshComponent& mesh, CameraComponent& Cam)
    {
        if (engine->world.CheckComponent<MaterialComponent>(EntityID))
        {
            MaterialComponent& Material = engine->world.GetComponent<MaterialComponent>(EntityID);

            if (Material.MaterialInstanceName.empty() == false)
            {
                // If Material Instance no Textures
                if (engine->gPBRManager->AllMaterialInstances[Material.MaterialInstanceName]->HasTexture == false)
                {
                    engine->gPBRManager->NonTexturedUniform(EntityID, Cam);
                }

                // If Material Instance have textures
                else
                {
                    engine->gPBRManager->TexturedUniform(EntityID, Cam);
                }
            }
            else
            {
                // If Do not have textures
                //if (engine->AssimpManager.Geometry[mesh.MeshName.data()]->NoTex && (!engine->world.CheckComponent<TextureComponent>(EntityID)))
                
                if (Material.NoTextures && (!engine->world.CheckComponent<TextureComponent>(EntityID)))
                {
                    engine->gPBRManager->SetAOConstant(shader, 1.0f);
                    engine->gPBRManager->SetMetallicConstant(shader, 0.5f);
                    engine->gPBRManager->SetRoughnessConstant(shader, 0.5f);
                    engine->gPBRManager->SetInstanceFlag(shader, false);
                    engine->gPBRManager->SetAlbedoConstant(shader, engine->AssimpManager.Geometry[mesh.MeshName.data()]->Diffuse);
                }
                // If we have textures
                else
                {
                    //unsigned int diffuseIdx = 0;
                    //unsigned int specularIdx = 0;
                    //unsigned int normalIdx = 0;

                    for (unsigned int it = 0; it < Material.HoldingTextures.size(); it++)
                    {
                        std::string name;
                        switch (Material.HoldingTextures[it].GetType())
                        {
                        case aiTextureType_DIFFUSE:
                            glActiveTexture(GL_TEXTURE0 + it);
                            break;
                        case aiTextureType_SPECULAR:
                            glActiveTexture(GL_TEXTURE0 + it);
                            break;
                        case aiTextureType_NORMALS:
                            glActiveTexture(GL_TEXTURE0 + it);
                            break;
                        }

                        GLuint IsNormalMap_ = shader.GetLocation("IsNormalMap");
                        glUniform1i(IsNormalMap_, Material.IsNormalMap);

                        engine->gPBRManager->UnBindMetallicTexture(shader);
                        engine->gPBRManager->UnBindRoughnessTexture(shader);
                        engine->gPBRManager->UnBindAOTexture(shader);
                        engine->gPBRManager->SetInstanceFlag(shader, true);

                        // No Normal Maps
                        if (Material.HoldingTextures[it].GetType() != aiTextureType_NORMALS)
                        {
                            GLuint albedoMap = shader.GetLocation("albedoMap");
                            glUniform1i(albedoMap, it);
                            Material.HoldingTextures[it].Bind();
                        }
                        // If we have normal Maps
                        else
                        {
                            GLuint normalMap = shader.GetLocation("normalMap");
                            glUniform1i(normalMap, it);
                            Material.HoldingTextures[it].Bind();
                        }
                    }
                }

                // reset
                glActiveTexture(GL_TEXTURE0);
            }
        }
    }

    bool AssimpModelManager::CheckGeometryExist(MeshComponent& in)
    {
        if (Geometry.find(in.MeshName.data()) == Geometry.end())
            return false;

        return true;
    }

    void AssimpModelManager::ChecModelkUniforms(Shader& _shdrpgm, CameraComponent& _camera, unsigned int ModelID, AABB_* box)
    {
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(_camera.camType));

        GLuint view = _shdrpgm.GetLocation("view");
        GLuint cameraPos = _shdrpgm.GetLocation("camPos");
        GLint projection = _shdrpgm.GetLocation("projection");
        GLuint model_ = _shdrpgm.GetLocation("model");

        auto& Transform = engine->world.GetComponent<TransformComponent>(ModelID);

        glm::mat4 mModelNDC;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, Transform.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(Transform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Transform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Transform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, Transform.scale.ConvertToGlmVec3Type());
        mModelNDC = _camera.projMtx * _camera.viewMtx * model;
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(_camera.viewMtx));
        GLCall(glUniform3f(cameraPos, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ()));

        if (box->DrawAABBS == true)
        {
            BoundingRegion br(Transform.position.ConvertToGlmVec3Type(), Transform.scale.ConvertToGlmVec3Type());
            box->AddInstance(br);
        }
    }
}