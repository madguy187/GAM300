#pragma once

#include "pch.h"
#include "System/System.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
	class TestSystem : public System
	{
	public:
		void Update()
		{
		  glBindFramebuffer(GL_FRAMEBUFFER, Graphics::m_frameBuffer->GetGameViewBuffer());
		  glClear(GL_COLOR_BUFFER_BIT);


		  glBindFramebuffer(GL_FRAMEBUFFER, 0);
		  glDisable(GL_DEPTH_TEST);
		  glClear(GL_COLOR_BUFFER_BIT);
		  glBindTexture(GL_TEXTURE_2D, Graphics::m_frameBuffer->GetTextureColourBuffer());

		  // render

		  glBindFramebuffer(GL_FRAMEBUFFER, 0);
		  glDisable(GL_DEPTH_TEST);
		  glClear(GL_COLOR_BUFFER_BIT);
		  glBindTexture(GL_TEXTURE_2D, Graphics::m_frameBuffer->m_data.TextureColourBuffer);

		 // ImGui::GetWindowDrawList()->AddImage(
			//(void*)(Graphics::m_frameBuffer->m_data.TextureColourBuffer),
			//ImVec2(ImGui::GetCursorScreenPos()),
			//ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowContentRegionMax().x,
			//  ImGui::GetCursorScreenPos().y + ImGui::GetWindowContentRegionMax().y), ImVec2(0, 1), ImVec2(1, 0));

		  ImGui::Begin("Game View");

		  //ImGui::End();
			//for (auto const& entity : mEntities)
			//{
			//	TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
			//	trans.x++;
			//}
		}
	};
}