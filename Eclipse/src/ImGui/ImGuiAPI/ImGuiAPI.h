#pragma once

#include "imgui.h"
#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

namespace Eclipse
{
	class ImGuiAPI
	{
	public:
		// Window Creations
		bool BeginMainWindow(const char* name);
		void EndMainWindow();
		bool BeginChildWindow(const char* name, ImVec2 size = ImVec2(0, 0), 
			                  bool border = false, ImGuiWindowFlags flag = 0);
		void EndChidWindow();
		ImVec2 GetWindowSize();
		void SetWindowSize(float width, float height);

		// Dynamic Widgets
		bool BeginPopUpButtonList(const char* name, const char* id);
		void EndPopUpButtonList();
		void BeginStyleVariant(ImGuiStyleVar id, float scale);
		void EndStyleVariant();
		bool BeginMenuBar();
		void EndMenuBar();
		bool BeginMenuComponents(const char* name);
		void EndMenuComponents();
		bool BeginComboList(const char* name, const char* previewCurrent, bool hideName = true,
			                ImGuiComboFlags flags = 0);
		void EndComboList();

		// Semi-Dynamic Widgets (Don't need an end function for it)
		bool CreateCollapsingHeader(const char* name);

		// Static Widgets
		// For Printing out on the windows
		void TextInt(const char* varname, const int& var);
		void TextFloat(const char* varname, const float& var);
		void TextDouble(const char* varname, const double& var);
		void TextSize_t(const char* varname, const size_t& var);
		void TextBool(const char* varname, const bool& var);
		void TextString(const char* varname, const std::string& var);
		void TextVec2(const char* varname, const ECVec2& var);
		void TextVec3(const char* varname, const ECVec3& var);
		void TextVec4(const char* varname, const ECVec4& var);
		void TextMat3(const char* varname, const ECMat3& var);
		void TextMat4(const char* varname, const ECMat4& var);
		void Text(const char* varname);
		// Manipulate Variables
		void InputInt(const char* name, int* var, bool hideName = true, int snapValue = 0);
		void InputFloat(const char* name, float* var, bool hideName = true, float snapValue = 0.0f);
		void InputFloat2(const char* name, float vector[2], bool hideName = true);
		void InputFloat3(const char* name, float vector[3], bool hideName = true);
		void InputFloat4(const char* name, float vector[4], bool hideName = true);
		void InputTextWithHint(const char* name, const char* hintText, char* buffer,
			                   size_t bufferSize, bool hideName = true);
		void InputText(const char* name, char* buffer, size_t bufferSize, bool hideName = true);

		void SliderInt(const char* name, int* var, bool hideName = true,
			           int minrange = 0.0f, int maxrange = 1000.0f);
		void SliderFloat(const char* name, float* var, bool hideName = true, 
			             float minrange = 0.0f, float maxrange = 1000.0f);
		void SliderFloat2(const char* name, float vector[2], bool hideName = true, 
			              float minrange = 0.0f, float maxrange = 1000.0f);
		void SliderFloat3(const char* name, float vector[3], bool hideName = true, 
			              float minrange = 0.0f, float maxrange = 1000.0f);
		void SliderFloat4(const char* name, float vector[4], bool hideName = true, 
			              float minrange = 0.0f, float maxrange = 1000.0f);
		// Misc
		bool CheckBoxBool(const char* name, bool* var, bool hideName = true);
		bool ButtonBool(const char* name);

		// Utilities
		// Mostly used together with the static widgets
		void InsertSameLine();
		void InsertHorizontalLineSeperator();
		void PushItemWidth(float value);

	private:
		std::string HideWidgetName(const char* name, bool Changing);
	};
}