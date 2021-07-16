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

void Eclipse::RenderSystem::Init()
{
	// Graphics Init =============================
	EDITOR_LOG_INFO("RenderSystem Init");
	engine->GraphicsManager.DebugPrintFrameBuffers();

	// Load All Models =============================
	engine->AssimpManager.LoadAllModels();

	// Create SKY =============================
	engine->GraphicsManager.CreateSky("src/Assets/Sky");

	// Create Grid =============================
	engine->GridMap.Init();
	engine->GridMap.DebugPrint();
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
	ProfilerWindow timer;
	timer.SetName({ SystemName::RENDER });
	timer.tracker.system_start = glfwGetTime();

	engine->GraphicsManager.UploadGammaCorrectionToShader();

	// SKY Reder =============================
	engine->GraphicsManager.RenderSky(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID());
	engine->GraphicsManager.RenderSky(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_LEFT)->GetFrameBufferID());
	engine->GraphicsManager.RenderSky(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_BOTTOM)->GetFrameBufferID());

	// (RENDERCOMPONENTS & TRANSFORMCOMPONENT) Render =============================
	for (auto const& entity : mEntities)
	{
		RenderComponent& _Sprites = engine->world.GetComponent<RenderComponent>(entity);
		engine->GraphicsManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);
		engine->GraphicsManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);
	}

	// CAMERA =============================
	engine->gDebugManager.DrawDebugShapes(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());

	// MODELS Render=============================
	//engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL);
	//engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL);
	//engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_TOP)->GetFrameBufferID(), GL_FILL);
	//engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_BOTTOM)->GetFrameBufferID(), GL_LINE);
	//engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_LEFT)->GetFrameBufferID(), GL_FILL);
	//engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_RIGHT)->GetFrameBufferID(), GL_LINE);

	engine->GridMap.DrawGrid();

	timer.tracker.system_end = glfwGetTime();
	timer.ContainerAddTime(timer.tracker);
}