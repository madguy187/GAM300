#include "pch.h"
#include "AssimpManager.h"

using namespace Eclipse;

void AssimpModelManager::CreateModel(std::string name, std::string FolderName)
{
    auto& GetWorld = engine->world;
    int ID = GetWorld.CreateEntity();

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
    AssimpModel* NewModel = new AssimpModel(glm::vec3(0.0f, -2.0f, -5.0f), glm::vec3(0.05f), false);
    NewModel->LoadAssimpModel(PathName);
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
    // Team using XML i think , so i prolly can do a xml file loading or some sort so people can mass load

    // Satic create first
    CreateModel("White Dog", "dog2");

    ENGINE_CORE_INFO("All Assimp Models Loaded");
}

void AssimpModelManager::DrawBuffers()
{
    auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");

    for (auto const& Models : AssimpModelContainer_)
    {
        auto& InvidualModels = *(Models.second);
        InvidualModels.Render(shdrpgm->second);
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
