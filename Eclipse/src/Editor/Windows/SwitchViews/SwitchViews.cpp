#include "pch.h"
#include "SwitchViews.h"

namespace Eclipse
{
	void SwitchViewsWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&SwitchViewsWindow::RunMainWindow, this));
	}

	SwitchViewsWindow::SwitchViewsWindow() :
		mViewportSize{ 0.0f, 0.0f }
	{
		Type = EditorWindowType::EWT_SWITCHVIEWS;
		WindowName = "Switch Views";
		IsVisible = false;

		m_frameBuffer_TOP = std::make_shared<FrameBuffer>
			(*engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SWITCHINGVIEWS_TOP));
		m_frameBuffer_LEFT = std::make_shared<FrameBuffer>
			(*engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SWITCHINGVIEWS_LEFT));
		m_frameBuffer_RIGHT = std::make_shared<FrameBuffer>
			(*engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SWITCHINGVIEWS_RIGHT));
		m_frameBuffer_BOTTOM = std::make_shared<FrameBuffer>
			(*engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SWITCHINGVIEWS_BOTTOM));
	}

	void SwitchViewsWindow::RunMainWindow()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			// Resize the framebuffer based on the size of the imgui window
			//m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y));
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		ChildSettings settings;
		settings.Name = "SwitchViewsFrameBuffers";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&SwitchViewsWindow::RunFrameBuffer, this));
	}

	void SwitchViewsWindow::RunFrameBuffer()
	{
		// TOP LEFT
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer_LEFT->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x / 2.0f, mViewportSize.y / 2.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ECGui::InsertSameLine();
		// TOP RIGHT
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer_TOP->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x / 2.0f, mViewportSize.y / 2.0f}, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// BOTTOM LEFT
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer_BOTTOM->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x / 2.0f, mViewportSize.y / 2.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ECGui::InsertSameLine();
		// BOTTOM RIGHT
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer_RIGHT->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x / 2.0f, mViewportSize.y / 2.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here
		}
	}
}
