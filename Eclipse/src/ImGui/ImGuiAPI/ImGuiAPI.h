#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace Eclipse
{
	class ImGuiAPI
	{
	public:
		// Window Creations
		static bool BeginMainWindow(const char* name);
		static bool BeginMainWindowWithFlag(const char* name, bool* p_open = NULL, 
			ImGuiWindowFlags flags = 0);
		static void EndMainWindow();
		static bool BeginChildWindow(const char* name, ImVec2 size = ImVec2(0, 0),
			                  bool border = false, ImGuiWindowFlags flag = 0);
		static void EndChildWindow();
		static ImVec2 GetWindowSize();
		static ImVec2 GetWindowPos();
		static ImVec2 GetCursorScreenPos();
		static float GetWindowHeight();
		static float GetWindowWidth();
		static void SetWindowSize(float width, float height);

		// Dynamic Widgets
		static bool BeginPopUpButtonList(const char* name, const char* id);
		static void EndPopUpButtonList();
		static void BeginStyleVariant(ImGuiStyleVar id, float scale);
		static void EndStyleVariant();
		static bool BeginMenuBar();
		static void EndMenuBar();
		static bool BeginMenuComponents(const char* name);
		static void EndMenuComponents();
		static bool BeginComboList(const char* name, const char* previewCurrent, bool hideName = true,
			                ImGuiComboFlags flags = 0);
		static void EndComboList();
		static bool BeginTreeNode(const char* name);
		static void EndTreeNode();

		// Semi-Dynamic Widgets (Don't need an end function for it)
		static bool CreateCollapsingHeader(const char* name);
		static bool CreateSelectableButton(const char* label, bool* active, ImGuiSelectableFlags flags = 0);
		static bool CreateMenuItem(const char* name, bool* open);

		// Static Widgets
		// For Printing out on the windows
		static void TextInt(const char* varname, const int& var);
		static void TextFloat(const char* varname, const float& var);
		static void TextDouble(const char* varname, const double& var);
		static void TextSize_t(const char* varname, const size_t& var);
		static void TextBool(const char* varname, const bool& var);
		static void TextString(const char* varname, const std::string& var);
		static void TextStaticString(const char* varname, const char* var);
		static void TextVec2(const char* varname, const ECVec2& var);
		static void TextVec3(const char* varname, const ECVec3& var);
		static void TextVec4(const char* varname, const ECVec4& var);
		static void TextMat3(const char* varname, const ECMat3& var);
		static void TextMat4(const char* varname, const ECMat4& var);
		static void Text(const char* varname);
		// Manipulate Variables
		static bool InputInt(const char* name, int* var, bool hideName = true, int snapValue = 0);
		static bool InputFloat(const char* name, float* var, bool hideName = true, float snapValue = 0.0f);
		static bool InputFloat2(const char* name, float vector[2], bool hideName = true);
		static bool InputFloat3(const char* name, float vector[3], bool hideName = true);
		static bool InputFloat4(const char* name, float vector[4], bool hideName = true);
		static bool InputTextWithHint(const char* name, const char* hintText, char* buffer,
			                   size_t bufferSize, bool hideName = true, ImGuiInputTextFlags flags = 0);
		static bool InputText(const char* name, char* buffer, size_t bufferSize,
			ImGuiInputTextFlags flag = 0, bool hideName = true);

		static bool SliderInt(const char* name, int* var, bool hideName = true,
			           int minrange = 0.0f, int maxrange = 1000.0f);
		static bool SliderFloat(const char* name, float* var, bool hideName = true,
			             float minrange = 0.0f, float maxrange = 1000.0f);
		static bool SliderFloat2(const char* name, float vector[2], bool hideName = true,
			              float minrange = 0.0f, float maxrange = 1000.0f);
		static bool SliderFloat3(const char* name, float vector[3], bool hideName = true,
			              float minrange = 0.0f, float maxrange = 1000.0f);
		static bool SliderFloat4(const char* name, float vector[4], bool hideName = true,
			              float minrange = 0.0f, float maxrange = 1000.0f);
		// Misc
		static bool CheckBoxBool(const char* name, bool* var, bool hideName = true);
		static bool ButtonBool(const char* name);

		// Utilities
		// Mostly used together with the static widgets
		static void InsertSameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
		static void InsertHorizontalLineSeperator();
		static void PushItemWidth(float value);
		static bool IsItemHovered();
		static void SetToolTip(const char* message);
		static void HelpMarker(const char* message);

		//Plot Widgets
		static void PlotHistogram(const char* label, const float* values, int values_count, 
			int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, 
			float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
	private:
		static std::string HideWidgetName(const char* name, bool Changing);
	};
}