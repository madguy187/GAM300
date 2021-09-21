#pragma once

#include "ImGui/ImGuiAPI/ImGuiAPI.h"

namespace Eclipse
{
	struct ChildSettings
	{
		const char* Name{ nullptr };
		ImVec2 Size{ 0,0 };
		bool HaveBorder{ false };
		ImGuiWindowFlags flags{ 0 };
	};

	struct ComboListSettings
	{
		const char* Name{ nullptr };
		bool HideName{ true };
		ImGuiWindowFlags flags{ 0 };
	};

	struct PopUpButtonSettings
	{
		const char* Name{ nullptr };
		const char* ID{ nullptr };
	};

	class ECGui
	{
	public:
		/*************************************************************************/
		/*                           WINDOW CREATIONS                            */
		/*************************************************************************/
		template <typename T, typename... Args>
		static void DrawMainWindow(const char* name, std::function<T> fn,
			Args&... args)
		{
			if (ImGuiAPI::BeginMainWindow(name))
				fn(args...);

			ImGuiAPI::EndMainWindow();
		}

		template <typename T, typename... Args>
		static void DrawChildWindow(ChildSettings settings, std::function<T> fn,
			Args&... args)
		{
			if (ImGuiAPI::BeginChildWindow(settings.Name, settings.Size, 
				                           settings.HaveBorder, settings.flags))
				fn(args...);

			ImGuiAPI::EndChildWindow();
		}

		static bool BeginMainWindow(const char* name);
		static bool BeginMainWindowWithFlag(const char* name, bool* p_open = NULL,
			ImGuiWindowFlags flags = 0);
		static void EndMainWindow();
		static bool BeginChildWindow(ChildSettings settings);
		static void EndChildWindow();
		static ImVec2 GetWindowSize();
		static ECVec2 GetWindowPos();
		static ECVec2 GetCursorScreenPos();
		static float GetWindowHeight();
		static float GetWindowWidth();
		static void SetWindowSize(float width, float height);

		/*************************************************************************/
		/*                           Dynamic Widgets                             */
		/*************************************************************************/
		template <typename T, typename... Args>
		static void BeginPopUpButtonList(PopUpButtonSettings settings,
			std::function<T> fn, Args&... args)
		{
			if (ImGuiAPI::BeginPopUpButtonList(settings.Name, settings.ID))
			{
				fn(args...);
				ImGuiAPI::EndPopUpButtonList();
			}
		}

		template <typename T, typename... Args>
		static void DrawMenuBar(std::function<T> fn, Args&... args)
		{
			if (ImGuiAPI::BeginMenuBar())
			{
				fn(args...);
				ImGuiAPI::EndMenuBar();
			}
		}

		template <typename T, typename... Args>
		static void DrawMenuComponent(const char* name,
			std::function<T> fn, Args&... args)
		{
			if (ImGuiAPI::BeginMenuComponents(name)) 
			{
				fn(args...);
				ImGuiAPI::EndMenuComponents();
			}
		}

		static bool BeginPopUpButtonList(PopUpButtonSettings settings);
		static void EndPopUpButtonList();
		static bool BeginMenuBar();
		static void EndMenuBar();
		static bool BeginMenuComponents(const char* name);
		static void EndMenuComponents();
		static void BeginStyleVariant(ImGuiStyleVar id, float scale);
		static void EndStyleVariant();
		static bool BeginTreeNode(const char* name);
		static void EndTreeNode();
		static bool CreateCollapsingHeader(const char* name, bool forInspector = false);
		static bool CreateSelectableButton(const char* label, bool* active, 
			                               ImGuiSelectableFlags flags = 0);
		static void CreateComboList(ComboListSettings settings,
			                        const std::vector<std::string>& vecStr,
			                        size_t& index);
		static bool CreateMenuItem(const char* name, bool* open);

		/*************************************************************************/
		/*                           Static Widgets                              */
		/*************************************************************************/
		// Special Draw Text Widget: To enable filter on its data members
		static bool DrawDataHeader(const char* varname, ImGuiTextFilter& filter);

		// Text Widgets - Template Overloading
		// Supports Int, Float, Double, Size_t, Bool, String, All our engine
		// Vectors and Matrices
		template <typename T>
		static void DrawTextWidget(const char* varname, const T&)
		{
			ImGuiAPI::Text(varname);
		}

		template <>
		static void DrawTextWidget(const char* varname, const int& var)
		{
			ImGuiAPI::TextInt(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const float& var)
		{
			ImGuiAPI::TextFloat(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const double& var)
		{
			ImGuiAPI::TextDouble(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const size_t& var)
		{
			ImGuiAPI::TextSize_t(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const bool& var)
		{
			ImGuiAPI::TextBool(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const std::string& var)
		{
			ImGuiAPI::TextString(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const ECVec2& var)
		{
			ImGuiAPI::TextVec2(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const ECVec3& var)
		{
			ImGuiAPI::TextVec3(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const ECVec4& var)
		{
			ImGuiAPI::TextVec4(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const ECMat3& var)
		{
			ImGuiAPI::TextMat3(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const ECMat4& var)
		{
			ImGuiAPI::TextMat4(varname, var);
		}

		// Supports Int and float only
		static bool DrawSliderIntWidget(const char* name, int* var,
			bool hideName = true, int minrange = 0.0f, int maxrange = 50.0f);
		static bool DrawSliderFloatWidget(const char* name, float* var,
			bool hideName = true, float minrange = 0.0f, float maxrange = 50.0f);
		static bool DrawSliderFloat2Widget(const char* name, ECVec2* vector,
			bool hideName = true, float minrange = 0.0f, float maxrange = 50.0f);
		static bool DrawSliderFloat3Widget(const char* name, ECVec3* vector,
			bool hideName = true, float minrange = 0.0f, float maxrange = 50.0f);
		static bool DrawSliderFloat4Widget(const char* name, ECVec4* vector,
			bool hideName = true, float minrange = 0.0f, float maxrange = 50.0f);

		// Input Widgets
		static bool DrawInputIntWidget(const char* name, int* var,
			bool hideName = true, int snapValue = 0);
		static bool DrawInputFloatWidget(const char* name, float* var,
			bool hideName = true, float snapValue = 0.0f);
		static bool DrawInputFloat2Widget(const char* name, ECVec2* vector,
			bool hideName = true);
		static bool DrawInputFloat3Widget(const char* name, ECVec3* vector,
			bool hideName = true);
		static bool DrawInputFloat4Widget(const char* name, ECVec4* vector,
			bool hideName = true);
		static bool DrawInputTextHintWidget(const char* name, const char* hintText,
			char* buffer, size_t bufferSize, bool hideName = true, ImGuiInputTextFlags flags = 0);
		static bool DrawInputTextWidget(const char* name, char* buffer,
			size_t bufferSize, ImGuiInputTextFlags flag = 0, bool hideName = true);

		// Misc
		static bool CheckBoxBool(const char* name, bool* var, bool hideName = true);
		static bool ButtonBool(const char* name);

		/*************************************************************************/
		/*                           Utilities                                   */
		/*************************************************************************/
		// Mostly used together with the static widgets
		static void InsertSameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
		static void InsertHorizontalLineSeperator();
		static void PushItemWidth(float value);
		static bool IsItemHovered();
		static void SetToolTip(const char* message);

		/*************************************************************************/
		/*                         Graph Widgets                                 */
		/*************************************************************************/
		static void PlotHistogram(const char* label, std::vector<float> values, 
			int values_offset = 0, const char* overlay_text = NULL, 
			float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), 
			int stride = sizeof(float));
	};
}
