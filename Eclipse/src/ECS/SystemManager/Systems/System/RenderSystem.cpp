#include "pch.h"
#include "RenderSystem.h"
#include "Graphics/Debugging/DebugRenderingManager.h"

//Components
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RenderComponent.h"
#include "AssimpModel/AssimpModel.h"

namespace Eclipse
{
	void RenderSystem::Init()
	{
		// Outlining Preparation ============================= 
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		// Graphics Init =============================
		EDITOR_LOG_INFO("RenderSystem Init");
		engine->GraphicsManager.DebugPrintFrameBuffers();

		// Load All Models =============================
		engine->AssimpManager.LoadAllModels();

		// Create SKY =============================
		engine->GraphicsManager.CreateSky("src/Assets/Sky");

		// Create Grid =============================
		engine->GraphicsManager.GridManager->Init();
		engine->GraphicsManager.GridManager->DebugPrint();

		box.init();
	}

	Signature RenderSystem::RegisterAll()
	{
		Signature SystemSignature;

		SystemSignature.set(engine->world.GetComponentType<TransformComponent>(), 1);
		SystemSignature.set(engine->world.GetComponentType<RenderComponent>(), 1);
		engine->world.RegisterSystemSignature<RenderSystem>(SystemSignature);

		return SystemSignature;
	}

	void RenderSystem::Update()
	{
		box.offsets.clear();
		box.sizes.clear();

		ProfilerWindow timer;
		timer.SetName({ SystemName::RENDER });
		timer.tracker.system_start = glfwGetTime();

		engine->GraphicsManager.UploadGlobalUniforms();

		if (engine->GraphicsManager.CheckRender == true)
		{
			// SKY Reder Start =============================
			engine->MaterialManager.DoNotUpdateStencil();
			engine->GraphicsManager.RenderSky(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());
			// SKY Reder End ===============================


			// Basic Primitives Render Start =============================
			for (auto const& entityID : mEntities) // - using RenderComponent and TransformComponent
			{
				RenderComponent& _Sprites = engine->world.GetComponent<RenderComponent>(entityID);

				engine->MaterialManager.UpdateStencilWithActualObject(entityID);
				engine->GraphicsManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);

				engine->MaterialManager.DoNotUpdateStencil();
				engine->GraphicsManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);
			}
			// Basic Primitives Render End ==============================


			// CAMERA Render Start =============================
			engine->MaterialManager.DoNotUpdateStencil();
			engine->gDebugManager.DrawDebugShapes(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());
			// CAMERA Render End ===============================


			// MODELS Render  Start =============================
			engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL, &box);

			engine->MaterialManager.DoNotUpdateStencil();
			engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), GL_FILL, &box);
			// MODELS Render  End ===============================

			// render boxes
			engine->MaterialManager.DoNotUpdateStencil();
			if (box.offsets.size() > 0)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());
				CameraComponent camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
				auto shdrpgm = Graphics::shaderpgms.find("AABB");
				shdrpgm->second.Use();

				GLint uniform_var_loc1 = shdrpgm->second.GetLocation("view");
				GLint uniform_var_loc2 = shdrpgm->second.GetLocation("projection");
				glm::mat4 _cameraprojMtx = glm::perspective(glm::radians(camera.fov), camera.aspect, camera.nearPlane, camera.farPlane);
				glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(camera.viewMtx));
				glUniformMatrix4fv(uniform_var_loc2, 1, GL_FALSE, glm::value_ptr(_cameraprojMtx));

				box.render(shdrpgm->second);
				shdrpgm->second.UnUse();
			}
		}

		timer.tracker.system_end = glfwGetTime();
		timer.ContainerAddTime(timer.tracker);
	}
}