#include "pch.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "ImGuizmo.h"
#include "ImGuiSetup.h"

namespace Eclipse
{
	void ImGuiSetup::Init(bool state)
	{
		if (state)
		{
			// Setup IMGUI Context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			
			// Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			// Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			// Enable Multi-Viewport / Platform Windows
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			// Color Style
			ImGui::StyleColorsCrimson();

			// When viewports are enabled, we tweak WindowRounding/WindowBG so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			// Setup Platform/Renderer Bindings 
			ImGui_ImplGlfw_InitForOpenGL(OpenGL_Context::GetWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 410");
		}
	}

	void ImGuiSetup::Begin(bool state)
	{
		if (state)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame(); 
			ImGuizmo::BeginFrame();
			ImGui::ShowDemoWindow(&state);
		}
	}

	void ImGuiSetup::End(bool state)
	{
		if (state)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2(static_cast<float>(OpenGL_Context::GetWidth()), 
				                    static_cast<float>(OpenGL_Context::GetHeight()));

			//Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}
	}

	void ImGuiSetup::Destroy(bool state)
	{
		if (state)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	}
}