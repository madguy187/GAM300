#include "pch.h"
#include "AssimpManager.h"

using namespace Eclipse;

void AssimpModelManager::CreateModel(std::string name, std::string FolderName)
{
    Entity ID = engine->world.CreateEntity();

    // add component here

    // Create path
    std::string PathName = ("src/Assets/ASSModels/" + FolderName + "/scene.gltf").c_str();

    // Test Path
    std::ifstream test(PathName);

    if (!test)
    {
        std::string Error = ("The file path " + PathName + " doesnt exist! ").c_str();
        ENGINE_LOG_ASSERT(false, Error);
        std::exit(EXIT_FAILURE);
    }

    // Initialise
    AssimpModel* NewModel = new AssimpModel(false);
    NewModel->LoadAssimpModel(PathName);

    // fbx file not working
    //NewModel->LoadAssimpModel("src/Assets/ASSModels/dog/1.FBX");
    NewModel->SetName(name);

    // Insert
    if (AssimpModelContainer_.insert(std::pair<unsigned int, AssimpModel*>(ID, NewModel)).second == true)
    {
        std::string Success = ("3D Model [" + name + "] Created and Inseted into Container Successfully! ").c_str();
        ENGINE_CORE_INFO(Success);
    }
}

void AssimpModelManager::LoadAllModels()
{
    CreateModel("White Dog", "dog");
    CreateModel("Black Dog", "dog");
    
    DebugPrint();

    ENGINE_CORE_INFO("All Assimp Models Loaded");
}

void AssimpModelManager::Draw(unsigned int FrameBufferID, GLenum Mode)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
    auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");

    for (auto const& Models : AssimpModelContainer_)
    {
        auto& InvidualModels = *(Models.second);
        InvidualModels.Render(shdrpgm->second, Mode);
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

void AssimpModelManager::DebugPrint()
{
    std::cout << std::endl;
    std::cout << "Container Size " << AssimpModelContainer_.size()  << std::endl;
    std::cout << "---------------------------------" << std::endl;

    for (auto const& Models : AssimpModelContainer_)
    {
        auto& InvidualModels = *(Models.second);
        std::cout << " Entity ID : " << Models.first << std::endl;
        std::cout << " Model Name : " << InvidualModels.GetName() << std::endl;
        std::cout << " Model Directory : " << InvidualModels.GetDirectory() << std::endl;
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

        // Everything Below this Comment is To be Removed !!
        TransformComponent& Transform = engine->world.GetComponent<TransformComponent>(Models.first);
        Transform.scale.setX(10);
        Transform.scale.setY(10);
        Transform.scale.setZ(10);
        Transform.rotation.setX(270);
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
