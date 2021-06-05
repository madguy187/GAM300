#pragma once

#include "ImGui/ImGuiAPI/ImGuiAPI.h"

namespace Eclipse
{
	static ImGuiAPI instance;

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
		const char* CurrentPreview{ nullptr };
		bool HideName{ true };
		ImGuiWindowFlags flags{ 0 };
	};

	struct PopUpButtonSettings
	{
		const char* Name{ nullptr };
		const char* ID{ nullptr };
	};

	class ECGui : public ImGuiAPI
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

		static ImVec2 GetWindowSize();
		static void SetWindowSize(float width, float height);

		/*************************************************************************/
		/*                           Dynamic Widgets                             */
		/*************************************************************************/
		template <typename T, typename... Args>
		static bool BeginPopUpButtonList(PopUpButtonSettings settings,
			std::function<T> fn, Args&... args)
		{
			if (ImGuiAPI::BeginPopUpButtonList(settings.Name, settings.ID))
				fn(args...);

			ImGuiAPI::EndPopUpButtonList();
		}

		template <typename T, typename... Args>
		static bool DrawMenuBar(std::function<T> fn, Args&... args)
		{
			if (ImGuiAPI::BeginMenuBar())
				fn(args...);

			ImGuiAPI::EndMenuBar();
		}

		template <typename T, typename... Args>
		static bool DrawMenuComponent(const char* name, 
			std::function<T> fn, Args&... args)
		{
			if (ImGuiAPI::BeginMenuComponents(name))
				fn(args...);

			ImGuiAPI::BeginMenuComponents();
		}

		static void BeginStyleVariant(ImGuiStyleVar id, float scale);
		static void EndStyleVariant();
		static bool BeginTreeNode(const char* name);
		static void EndTreeNode();
		static bool CreateCollapsingHeader(const char* name);
		static bool CreateSelectableButton(const char* label, bool* active, 
			                               ImGuiSelectableFlags flags = 0);
		static void CreateComboList(ComboListSettings settings,
			                        const std::vector<std::string>& vecStr,
			                        size_t& index);

		/*************************************************************************/
		/*                           Static Widgets                              */
		/*************************************************************************/
		// Text Widgets - Template Overloading
		// Supports Int, Float, Double, Size_t, Bool, String, All our engine
		// Vectors and Matrices
		template <typename T>
		static void DrawTextWidget(const char* varname, const T&)
		{
			instance.Text(varname);
		}

		template <>
		static void DrawTextWidget(const char* varname, const int& var)
		{
			instance.TextInt(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const float& var)
		{
			instance.TextFloat(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const double& var)
		{
			instance.TextDouble(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const size_t& var)
		{
			instance.TextSize_t(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const bool& var)
		{
			instance.TextBool(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const std::string& var)
		{
			instance.TextString(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const ECVec2& var)
		{
			instance.TextVec2(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const ECVec3& var)
		{
			instance.TextVec3(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const ECVec4& var)
		{
			instance.TextVec4(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const ECMat3& var)
		{
			instance.TextMat3(varname, var);
		}

		template <>
		static void DrawTextWidget(const char* varname, const ECMat4& var)
		{
			instance.TextMat4(varname, var);
		}

		// Supports Int and float only
		static void DrawSliderIntWidget(const char* name, int* var, 
			bool hideName = true, int minrange = 0.0f, int maxrange = 1000.0f);
		static void DrawSliderFloatWidget(const char* name, float* var,
			bool hideName = true, float minrange = 0.0f, float maxrange = 1000.0f);
		static void DrawSliderFloat2Widget(const char* name, float vector[2],
			bool hideName = true, float minrange = 0.0f, float maxrange = 1000.0f);
		static void DrawSliderFloat3Widget(const char* name, float vector[3],
			bool hideName = true, float minrange = 0.0f, float maxrange = 1000.0f);
		static void DrawSliderFloat4Widget(const char* name, float vector[4],
			bool hideName = true, float minrange = 0.0f, float maxrange = 1000.0f);

		// Input Widgets
		static void DrawInputIntWidget(const char* name, int* var,
			bool hideName = true, int snapValue = 0);
		static void DrawInputFloatWidget(const char* name, float* var,
			bool hideName = true, float snapValue = 0.0f);
		static void DrawInputFloat2Widget(const char* name, float vector[2],
			bool hideName = true);
		static void DrawInputFloat3Widget(const char* name, float vector[3],
			bool hideName = true);
		static void DrawInputFloat4Widget(const char* name, float vector[4],
			bool hideName = true);
		static void DrawInputTextHintWidget(const char* name, const char* hintText,
			char* buffer, size_t bufferSize, bool hideName = true);
		static void DrawInputTextWidget(const char* name, char* buffer,
			size_t bufferSize, bool hideName = true);

		// Misc
		static bool CheckBoxBool(const char* name, bool* var, bool hideName = true);
		static bool ButtonBool(const char* name);

		/*************************************************************************/
		/*                           Utilities                                   */
		/*************************************************************************/
		// Mostly used together with the static widgets
		static void InsertSameLine();
		static void InsertHorizontalLineSeperator();
		static void PushItemWidth(float value);
		static bool IsItemHovered();
		static void SetToolTip(const char* message);
	};
}
