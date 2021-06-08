#include "pch.h"
#include "GameView.h"

namespace Eclipse
{
	void eGameView::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&eGameView::InitilializeFrameBuffer, this));
	}

	eGameView::eGameView() :
		mViewportSize{ 0.0f, 0.0f }
	{
		Type = EditorWindowType::GAMEVIEW;
		WindowName = "Game View";

		m_frameBuffer = std::make_shared<FrameBuffer>(*engine->gGraphics.mRenderContext.GetFramebuffer(FrameBufferMode::GAMEVIEW));
	}

	void eGameView::InitilializeFrameBuffer()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			// Resize the framebuffer based on the size of the imgui window
			m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y));
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		ChildSettings settings;
		settings.Name = "GameViewFrameBuffer";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&eGameView::RunFrameBuffer, this));
	}

	void eGameView::RunFrameBuffer()
	{
		// Set Image size
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())), 
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here
		}
	}
}