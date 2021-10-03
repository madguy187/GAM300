#include "pch.h"
#include "AssimpManager.h"

namespace Eclipse
{
    typedef std::multimap<std::string, Texture>::iterator MMAPIterator;

    void AssimpModelManager::Init()
    {
        LoadCompilers();
        PrintLoadedModels();
    }

    void AssimpModelManager::ExecuteCompiler()
    {
        // I Will Load First
        system("Compiler.exe");
        EDITOR_LOG_INFO("Compiler Finish Excution");
    }

    void AssimpModelManager::LoadCompilers()
    {
        std::thread CompilerWait{ &AssimpModelManager::ExecuteCompiler };
        CompilerWait.join();

        // Geometry Compiler
        LoadGeometry();
        // Parent Model Mappings
        LoadPrefabs();
        // Texture Compiler
        LoadTextures();
        LoadBasicTextures();

        if (CheckCompilers())
        {
            EDITOR_LOG_INFO("All Compilers Loaded");
        }
    }

    // Hope its not a fail...
    void AssimpModelManager::HotReload()
    {
        if (HotReloadFlag == false)
        {
            AllMeshNames.clear();
            Geometry.clear();
            Prefabs.clear();
            Graphics::textures.clear();

            std::thread CompilerWait{ &AssimpModelManager::ExecuteCompiler };
            CompilerWait.join();

            // Geometry Compiler
            LoadGeometry();
            // Parent Model Mappings
            LoadPrefabs();
            // Texture Compiler
            LoadTextures();
            LoadBasicTextures();
        }
    }

    void AssimpModelManager::HotReloadTetxures()
    {
        Graphics::textures.clear();

        // I Will Load First
        std::thread CompilerWait{ &AssimpModelManager::ExecuteCompiler };
        CompilerWait.join();

        engine->AssimpManager.LoadTextures();
        engine->AssimpManager.LoadBasicTextures();
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
        int Index = 0;

        std::pair<MMAPIterator, MMAPIterator> result = Graphics::textures.equal_range(passkey);

        for (MMAPIterator it = result.first; it != result.second; it++)
        {
            in.HoldingTextures.push_back(it->second);
        }
    }

    void AssimpModelManager::InsertTextures(std::string& NameofModel, std::unique_ptr<Texture> in, unsigned int MeshId)
    {
        LoadedTexturesV2[NameofModel][MeshId].push_back(std::move(in));
    }

    void AssimpModelManager::PrintLoadedModels()
    {
        //PrintOutModelsLoadedOnce();
        //PrintOutModelTextureMap();
        //PrintOutModelMap();
        //PrintOutAllTextures();
        //PrintOutAllMeshes();
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

            //Mesh.VBO = Geometry[in]->VBO;
            //Mesh.VAO = Geometry[in]->VAO;
            //Mesh.EBO = Geometry[in]->EBO;
            //Mesh.NoTex = Geometry[in]->NoTex;
            //Mesh.Diffuse = Geometry[in]->Diffuse;
            //Mesh.Specular = Geometry[in]->Specular;
            //Mesh.Ambient = Geometry[in]->Ambient;
            //Mesh.Vertices = Geometry[in]->Vertices;
            //Mesh.Indices = Geometry[in]->Indices;
        }
    }

    void AssimpModelManager::Render(Shader& shader, GLenum mode, unsigned int id, MeshComponent& in)
    {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, mode);

        // If dont have textures ( Flagged as True )
        if (engine->AssimpManager.Geometry[in.MeshName.data()]->NoTex && (!engine->world.CheckComponent<TextureComponent>(id)))
        {
            GLint uniform_var_loc1 = shader.GetLocation("BasicPrimitives");
            GLint uniform_var_loc2 = shader.GetLocation("uColor");
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
                        GLuint dsa = shader.GetLocation("noTex");

                        glUniform1i(dsa, false);
                        glUniform1i(uniform_var_loc3, true);
                        glUniform1i(diff0, it);
                        glUniform1i(spec, it);
                        tex.HoldingTextures[it].Bind();
                    }
                    else
                    {
                        GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
                        GLuint CheckNoTex = shader.GetLocation("noTex");
                        GLuint CheckNormapMap = shader.GetLocation("CheckNormapMap");
                        GLuint normal0 = shader.GetLocation("normal0");
                        glUniform1i(uniform_var_loc3, true);
                        glUniform1i(CheckNoTex, false);
                        glUniform1i(CheckNormapMap, true);
                        glUniform1i(normal0, it);
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

        // EBO stuff
        glBindVertexArray(engine->AssimpManager.Geometry[in.MeshName.data()]->VAO);
        glDrawElements(GL_TRIANGLES, engine->AssimpManager.Geometry[in.MeshName.data()]->Indices.size(), GL_UNSIGNED_INT, 0);
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
        glDrawElements(GL_TRIANGLES, engine->AssimpManager.Geometry[in.MeshName.data()]->Indices.size(), GL_UNSIGNED_INT, 0);
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
            std::string MeshName = Mesh.MeshName.data();
            engine->AssimpManager.SetTexturesForModel(tex, MeshName);
        }

    }

    void AssimpModelManager::LoadGeometry()
    {
        // Loading Eclipse File
        std::fstream GeometryFileRead;

        GeometryFileRead.open("../Compiler/CompilerKeyFiles/GeometryFile/Geometry.eclipse",
            std::ios::in |
            std::ios::binary);

        if (GeometryFileRead.fail())
        {
            std::cout << "Fail To Open Geometry File" << std::endl << std::endl;
            return;
        }

        unsigned int TotalNumberOfModels = 0;
        unsigned int VerticesSize = 0;
        unsigned int IndicesSize = 0;

        // See how many Models
        GeometryFileRead.read(reinterpret_cast<char*>(&TotalNumberOfModels), sizeof(TotalNumberOfModels));

        for (int i = 0; i < TotalNumberOfModels; i++)
        {
            VerticesSize = 0;
            IndicesSize = 0;

            Mesh B;

            auto hh = sizeof(B);

            GeometryFileRead.read(reinterpret_cast<char*>(&B), offsetof(Mesh, Vertices));

            GeometryFileRead.read(reinterpret_cast<char*>(&VerticesSize), sizeof(VerticesSize));
            B.Vertices.resize(VerticesSize);
            GeometryFileRead.read(reinterpret_cast<char*>(B.Vertices.data()), sizeof(Vertex) * VerticesSize);

            GeometryFileRead.read(reinterpret_cast<char*>(&IndicesSize), sizeof(IndicesSize));

            B.Indices.resize(IndicesSize);
            GeometryFileRead.read(reinterpret_cast<char*>(B.Indices.data()), sizeof(unsigned int) * IndicesSize);


            if (B.NoTex == false)
            {
                Mesh NewMesh(B.Vertices, B.Indices, B.MeshName.data(), B.Textures);
                std::string name = B.MeshName.data();
                Geometry.emplace(name, std::make_unique<Mesh>(NewMesh));
                AllMeshNames.push_back(name);
            }
            else
            {
                Mesh NewMesh(B.Vertices, B.Indices, B.Diffuse, B.Specular, B.Ambient, B.NoTex, B.MeshName.data());
                std::string name = B.MeshName.data();
                Geometry.emplace(name, std::make_unique<Mesh>(NewMesh));
                AllMeshNames.push_back(name);
            }
        }

        GeometryFileRead.close();
    }

    void AssimpModelManager::LoadPrefabs()
    {
        // Prefab Container Names
        std::ofstream PrefabsFileWrite;
        std::fstream PrefabsFileRead;

        PrefabsFileRead.open("../Compiler/CompilerKeyFiles/PrefabsFile/Prefabs.eclipse",
            std::ios::in |
            std::ios::binary);

        if (PrefabsFileRead.fail())
        {
            std::cout << "Fail To Open Geometry File" << std::endl << std::endl;
            return;
        }

        int TotalNumberOfPrefabs = 0;

        // See how many Prefabs
        PrefabsFileRead.read(reinterpret_cast<char*>(&TotalNumberOfPrefabs), sizeof(int));

        for (int i = 0; i < TotalNumberOfPrefabs; i++)
        {
            std::array<char, 128> ParentName;
            PrefabsFileRead.read(reinterpret_cast<char*>(&ParentName), sizeof(ParentName));
            ParentName[ParentName.size() - 1] = '\0';

            int NumberOfSubMeshes = 0;
            PrefabsFileRead.read(reinterpret_cast<char*>(&NumberOfSubMeshes), sizeof(NumberOfSubMeshes));

            for (int i = 0; i < NumberOfSubMeshes; i++)
            {
                std::array<char, 128> MeshName;
                PrefabsFileRead.read(reinterpret_cast<char*>(&MeshName), sizeof(MeshName));
                MeshName[MeshName.size() - 1] = '\0';

                Prefabs[ParentName.data()].push_back(MeshName.data());
            }

            AllMeshNames.push_back(ParentName.data());
        }
        PrefabsFileRead.close();
    }

    void AssimpModelManager::LoadTextures()
    {
        std::ofstream TextureFileWrite;
        std::fstream TextureFileRead;

        TextureFileRead.open("../Compiler/CompilerKeyFiles/TextureFile/Texture.eclipse",
            std::ios::in |
            std::ios::binary);

        if (TextureFileRead.fail())
        {
            std::cout << "Fail To Open Texture File" << std::endl << std::endl;
            return;
        }

        // Number Of Textures
        int NumberOfTextures = 0;
        TextureFileRead.read(reinterpret_cast<char*>(&NumberOfTextures), sizeof(NumberOfTextures));

        for (int i = 0; i < NumberOfTextures; i++)
        {
            // Mesh Name
            std::array<char, 128> MeshName;
            TextureFileRead.read(reinterpret_cast<char*>(&MeshName), sizeof(MeshName));

            // Number Of Textures
            int TextureType = 0;
            TextureFileRead.read(reinterpret_cast<char*>(&TextureType), sizeof(TextureType));

            // Texture Directory
            std::array<char, 128> TextureDirectory;
            TextureFileRead.read(reinterpret_cast<char*>(&TextureDirectory), sizeof(TextureDirectory));

            // Texture DirecPathtory
            std::array<char, 128> TexturePath;
            TextureFileRead.read(reinterpret_cast<char*>(&TexturePath), sizeof(TexturePath));

            Texture tex(TextureDirectory.data(), TexturePath.data(), static_cast<aiTextureType>(TextureType));
            tex.Load(false);
            Graphics::textures.emplace(MeshName.data(), tex);
        }
        TextureFileRead.close();
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

    void AssimpModelManager::ClearGeometry()
    {
        Geometry.clear();
    }

    void AssimpModelManager::LoadBasicTextures()
    {
        std::fstream TextureFileRead;

        TextureFileRead.open("../Compiler/CompilerKeyFiles/BasicTextureFile/Texture.eclipse",
            std::ios::in |
            std::ios::binary);

        if (TextureFileRead.fail())
        {
            std::cout << "Fail To Open Texture File" << std::endl << std::endl;
            return;
        }

        // Number Of Textures
        int NumberOfBasicTextures = 0;
        TextureFileRead.read(reinterpret_cast<char*>(&NumberOfBasicTextures), sizeof(NumberOfBasicTextures));

        for (int i = 0; i < NumberOfBasicTextures; i++)
        {
            // Texture Name
            std::array<char, 128> TextureName;
            TextureFileRead.read(reinterpret_cast<char*>(&TextureName), sizeof(TextureName));

            // Texture DirecPathtory
            std::array<char, 128> TexturePath;
            TextureFileRead.read(reinterpret_cast<char*>(&TexturePath), sizeof(TexturePath));

            Texture tex(TexturePath.data());
            Graphics::textures.emplace(TextureName.data(), tex);
        }

        TextureFileRead.close();
        ///////////////////////////
    }

    bool AssimpModelManager::GetHotReloadFlag()
    {
        return false;
    }

    void AssimpModelManager::ResetHotReloadFlag()
    {
        HotReloadFlag = false;
    }

    bool AssimpModelManager::CheckCompilers()
    {
        if (Geometry.size() == 0 || Prefabs.size() == 0)
        {
            EDITOR_LOG_WARN("No Geometries Loaded , Please Run Geometry Compiler");
            return false;
        }

        return true;
    }

    void AssimpModelManager::CreateModel(unsigned int ID, const std::string& ModelName)
    {
        if (CheckCompilers())
        {
            if (Prefabs.find(ModelName) == Prefabs.end())
            {
                // Cannot Find this as a parent
                std::string Name = ModelName;
                engine->world.AddComponent(ID, MeshComponent{});
                engine->world.AddComponent(ID, ModeLInforComponent{});
                engine->world.AddComponent(ID, MaterialComponent{ MaterialModelType::MT_MODELS3D });
                //engine->world.AddComponent(ID, TextureComponent{});
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
                    auto MeshID = engine->editorManager->CreateDefaultEntity(EntityType::ENT_UNASSIGNED);

                    engine->world.AddComponent(MeshID, MeshComponent{});
                    engine->world.AddComponent(MeshID, ModeLInforComponent{});
                    engine->world.AddComponent(MeshID, MaterialComponent{ MaterialModelType::MT_MODELS3D });
                    //engine->world.AddComponent(MeshID, TextureComponent{});

                    SetSingleMesh(MeshID, name);
                }
            }
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

    std::vector<std::string>& AssimpModelManager::GetMeshNames()
    {
        return AllMeshNames;
    }

    std::vector<std::string>& AssimpModelManager::GetPrimitiveNames()
    {
        return AllPrimitiveModelsNames;
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

    std::string AssimpModelManager::GetKey(const std::string& in)
    {
        return AssimpLoadedModels[in]->GetName();
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
}
