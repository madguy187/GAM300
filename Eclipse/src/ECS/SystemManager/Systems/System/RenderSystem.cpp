#include "pch.h"
#include "RenderSystem.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/InputHandler/AllInputKeyCodes.h"
#include "Graphics/Debugging/DebugRenderingManager.h"

//Components
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RenderComponent.h"


#include "AssimpModel/AssimpModel.h"

#include "SparseSet/SparseSet.hpp"

//Cube m;
std::vector<AssimpModel> test;

void Eclipse::RenderSystem::Init()
{
    ENGINE_CORE_INFO("RenderSystem Init");
    engine->gGraphics.pre_render();

    engine->AssimpManager.LoadAllModels();

    //AssimpModel m(glm::vec3(0.0f, -2.0f, -5.0f), glm::vec3(0.05f), false);
    //std::string name = "dog";
    //m.loadAssimpModel(("src/Assets/ASSModels/" + name + "/scene.gltf").c_str());
    //test.push_back(m);
    //engine->gGraphics.ModelContainer.push_back(&m);
}

Signature Eclipse::RenderSystem::RegisterAll()
{
    Signature SystemSignature;

    SystemSignature.set(engine->world.GetComponentType<TransformComponent>(), 1);
    SystemSignature.set(engine->world.GetComponentType<RenderComponent>(), 1);
    engine->world.RegisterSystemSignature<RenderSystem>(SystemSignature);

    return SystemSignature;
}

void Eclipse::RenderSystem::Update()
{
    //Loop
    for (auto const& entity : mEntities)
    {
        RenderComponent& _Sprites = engine->world.GetComponent<RenderComponent>(entity);
        engine->gGraphics.DrawBuffers(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);
        engine->gGraphics.DrawBuffers(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);
    }

    engine->gDebugManager.DrawDebugShapes(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());

    engine->AssimpManager.DrawBuffers(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL);

    //auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");
    //test[0].render(shdrpgm->second);
}