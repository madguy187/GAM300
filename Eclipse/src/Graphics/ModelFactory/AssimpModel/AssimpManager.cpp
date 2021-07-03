#include "pch.h"
#include "AssimpManager.h"

using namespace Eclipse;

void AssimpModelManager::CreateModel(std::string name, std::string FolderName)
{
    auto& GetWorld = engine->world;

    // Create path
    std::string PathName = ("src/Assets/ASSModels/dog/scene.gltf");

    // Initialise
    AssimpModel* NewModel = new AssimpModel(glm::vec3(0.0f, -2.0f, -5.0f), glm::vec3(0.05f), false);
    NewModel->loadAssimpModel(PathName);
    NewModel->NameOfModel = name;

    if (AssimpModelContainer_.insert(std::pair<unsigned int, AssimpModel*>(GetWorld.CreateEntity(), NewModel)).second == true)
    {
        ENGINE_CORE_INFO("3D Model Created and Inseted into Container Successfully");
    }
}

void AssimpModelManager::LoadAllModels()
{
    // Team using XML i think , so i prolly can do a xml file loading or some sort so people can mass load

    // Satic create first
    CreateModel("White Dog", "dog");
}

void AssimpModelManager::DrawBuffers(unsigned int FrameBufferID, GLenum mode)
{
    auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");
    glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

    AssimpModelContainer_[0]->render(shdrpgm->second);

    //for (auto const& Models : AssimpModelContainer_)
    //{
    //    auto& InvidualModels = *(Models.second);
    //    InvidualModels.render(shdrpgm->second);
    //}
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
            return;
        }
    }
}
