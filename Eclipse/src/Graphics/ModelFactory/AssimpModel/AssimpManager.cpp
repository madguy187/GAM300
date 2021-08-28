#include "pch.h"
#include "AssimpManager.h"

namespace Eclipse
{
    void AssimpModelManager::CreateModel(std::string name, std::string FolderName, std::string filename)
    {
        Entity ID = engine->world.CreateEntity();

        // Add Required Components
        engine->world.AddComponent(ID, MaterialComponent{});
        engine->world.AddComponent(ID, TransformComponent{});
        engine->world.AddComponent(ID, EntityComponent{ EntityType::ENT_UNASSIGNED, name , true });

        // Manually adding to hierachy List
        // ----------------------------------------------------------------------------------------------------------
        engine->editorManager->EntityHierarchyList_.push_back(ID);
        engine->editorManager->EntityToTypeMap_.insert(std::pair<Entity, EntityType>(ID, EntityType::ENT_UNASSIGNED));

        TransformComponent& Transform = engine->world.GetComponent<TransformComponent>(ID);
        Transform.scale.setX(5);
        Transform.scale.setY(5);
        Transform.scale.setZ(5);

        // intermittent fix did not work
        //Transform.rotation.setX(-270);

        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::Models3D;
        // ----------------------------------------------------------------------------------------------------------


        // Create path
        std::string PathName = ("src/Assets/ASSModels/" + FolderName + "/" + filename).c_str();

        // Test Path
        TestPath(PathName);

        // Always set False because we have textures
        AssimpModel* NewModel = new AssimpModel(false);
        NewModel->LoadAssimpModel(PathName);
        NewModel->SetName(name);
        NewModel->SetModelType(ModelType::ANIMAL);

        // Insert
        if (AssimpModelContainer_.insert(std::pair<unsigned int, AssimpModel*>(ID, NewModel)).second == true)
        {
            std::string Success = ("3D Model [" + name + "] Created and Inseted into Container Successfully! ").c_str();
            ENGINE_CORE_INFO(Success);
        }

        // ----------------------------------------------------------------------------------------------------------
        if (engine->MaterialManager.InitRegisterHighlight == true)
        {
            MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
            engine->MaterialManager.RegisterForHighlighting(mat, ID);
        }
        // ----------------------------------------------------------------------------------------------------------

        //Test Code
        if (0)
        {
            AssimpModel* ONE = new AssimpModel(false);
            ONE->LoadAssimpModel(PathName);
            ONE->SetName(name);
            ONE->SetModelType(ModelType::ANIMAL);

            AssimpModel* TWO = new AssimpModel(false);
            TWO->LoadAssimpModel(PathName);
            TWO->SetName(name);

            AssimpModel* Three = new AssimpModel(false);
            Three->LoadAssimpModel(PathName);
            Three->SetName(name);

            ONE->test.maxX = 15;
            ONE->test.maxY = 15;
            ONE->test.maxZ = 15;
            ONE->test.minX = -15;
            ONE->test.minY = -15;
            ONE->test.minZ = -15;
            ONE->test.maxX = 15;
            ONE->test.EntityID = 10;

            TWO->test.maxX = 5;
            TWO->test.maxY = 5;
            TWO->test.maxZ = 5;
            TWO->test.minX = -5;
            TWO->test.minY = -5;
            TWO->test.minZ = -5;
            TWO->test.EntityID = 11;

            Three->test.maxX = -20;
            Three->test.maxY = -20;
            Three->test.maxZ = -20;
            Three->test.minX = -50;
            Three->test.minY = -50;
            Three->test.minZ = -50;
            Three->test.EntityID = 12;

            AssimpModelContainer AABBCC;

            AABBCC.insert(std::pair<unsigned int, AssimpModel*>(ONE->test.EntityID, ONE));
            AABBCC.insert(std::pair<unsigned int, AssimpModel*>(TWO->test.EntityID, TWO));
            AABBCC.insert(std::pair<unsigned int, AssimpModel*>(Three->test.EntityID, Three));

            std::shared_ptr<AssimpModel>first(ONE);
            std::shared_ptr<AssimpModel>second(TWO);
            std::shared_ptr<AssimpModel>third(Three);

            // tiles
            engine->test.InsertObject(first);
            engine->test.InsertObject(second);
            engine->test.InsertObject(third);

            //pass in model to check with the grid
            auto& aabbCollisions = engine->test.queryOverlaps(first.get()->getAABB());

            std::cout << AABBCC[aabbCollisions[0]]->GetName() << std::endl;
        }

    }

    void AssimpModelManager::LoadAllModels()
    {
        DebugPrint();

        ENGINE_CORE_INFO("All Assimp Models Loaded");
        EDITOR_LOG_INFO("All Necessary Models Loaded");
    }

    void AssimpModelManager::Draw(unsigned int FrameBufferID, GLenum Mode, AABB_* box, CameraComponent::CameraType _camType)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(_camType));

        glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
        auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");

        shdrpgm->second.Use();

        for (auto const& Models : AssimpModelContainer_)
        {
            auto& ID = Models.first;
            auto& InvidualModels = *(Models.second);

            engine->MaterialManager.UpdateStencilWithActualObject(ID);

            // Check Main Uniforms For each Model
            // Translation done here for each model
            CheckUniformLoc(shdrpgm->second, _camera, FrameBufferID, ID, box);

            // Render
            InvidualModels.Render(shdrpgm->second, Mode, FrameBufferID);
        }

        shdrpgm->second.UnUse();
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
                InvidualModels.Render(shdrpgm->second, Mode, FrameBufferID);
            }

            shdrpgm->second.UnUse();
        }
    }

    void AssimpModelManager::CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int FrameBufferID, unsigned int ModelID, AABB_* box)
    {
        MaterialComponent& material = engine->world.GetComponent<MaterialComponent>(ModelID);
        GLint uniform_var_loc1 = _shdrpgm.GetLocation("material.shininess");
        GLint uniform_var_loc2 = _shdrpgm.GetLocation("material.MaximumShininess");

        GLCall(glUniform1f(uniform_var_loc1, material.shininess));
        GLCall(glUniform1f(uniform_var_loc2, material.MaximumShininess));


        TransformComponent& Transform = engine->world.GetComponent<TransformComponent>(ModelID);
        GLint uModelToNDC_ = _shdrpgm.GetLocation("uModelToNDC");
        GLuint model_ = _shdrpgm.GetLocation("model");
        GLuint dsa = _shdrpgm.GetLocation("noTex");

        if (uModelToNDC_ >= 0)
        {
            glm::mat4 mModelNDC;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, Transform.position.ConvertToGlmVec3Type());
            model = glm::rotate(model, glm::radians(Transform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(Transform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(Transform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, Transform.scale.ConvertToGlmVec3Type());

            BoundingRegion br(Transform.position.ConvertToGlmVec3Type(), Transform.scale.ConvertToGlmVec3Type());
            box->AddInstance(br, Transform.position.ConvertToGlmVec3Type());

            mModelNDC = _camera.projMtx * _camera.viewMtx * model;
            glUniformMatrix4fv(uModelToNDC_, 1, GL_FALSE, glm::value_ptr(mModelNDC));
            glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        }

        glUniform1i(dsa, 0);
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

    void AssimpModelManager::DebugPrint()
    {
        std::cout << std::endl;
        std::cout << "Container Size " << AssimpModelContainer_.size() << std::endl;
        std::cout << "---------------------------------" << std::endl;

        for (auto const& Models : AssimpModelContainer_)
        {
            auto& InvidualModels = *(Models.second);
            std::cout << " Entity ID : " << Models.first << std::endl;
            std::cout << " Model Name : " << InvidualModels.GetName() << std::endl;
            std::cout << " Model Directory : " << InvidualModels.GetDirectory() << std::endl;
            std::cout << " Number of Textures : " << InvidualModels.GetNumberOfTextures() << std::endl;
            InvidualModels.GetTextureNames();
            std::cout << std::endl;
        }

        std::cout << "---------------------------------" << std::endl;
        std::cout << std::endl;
    }

    void AssimpModelManager::AddComponents()
    {
        for (auto const& Models : AssimpModelContainer_)
        {
            auto& InvidualModels = *(Models.second);
            std::cout << "Added Transform Component For 3DModel ID : " << Models.first << std::endl;
            engine->world.AddComponent(Models.first, TransformComponent{});
            engine->world.AddComponent(Models.first, EntityComponent{ EntityType::ENT_UNASSIGNED, InvidualModels.GetName() , true });

            // Manually adding to hierachy List
            engine->editorManager->EntityHierarchyList_.push_back(Models.first);
            engine->editorManager->EntityToTypeMap_.insert(std::pair<Entity, EntityType>(Models.first, EntityType::ENT_UNASSIGNED));

            // Everything Below this Comment is To be Removed !!
            TransformComponent& Transform = engine->world.GetComponent<TransformComponent>(Models.first);
            Transform.scale.setX(10);
            Transform.scale.setY(10);
            Transform.scale.setZ(10);

            // Sit properly ( intermittent fix not working )
            //Transform.rotation.setX(270);
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

    AssimpModelManager::~AssimpModelManager()
    {
        //for (auto i : AssimpModelContainer_)
        //{
        //    if (i.second != nullptr)
        //    {
        //        delete i.second;
        //    }
        //}
    }

    void AssimpModelManager::addSpriteToCollisionSet(const std::shared_ptr<AssimpModel>& sprite)
    {
        engine->test.InsertObject(sprite);
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
}
