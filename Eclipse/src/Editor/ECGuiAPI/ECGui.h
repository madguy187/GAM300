#pragma once

#include "ImGui/ImGuiAPI/ImGuiAPI.h"

#define EMPTY_STRING ""

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
		static ImVec2 GetContentRegionAvail();
		static ImGuiViewport* GetMainViewport();
		static ImDrawList* GetWindowDrawList();
		static ImVec2 GetItemRectMin();
		static ImVec2 GetItemRectMax();
		static float GetWindowHeight();
		static float GetWindowWidth();
		static void SetWindowSize(float width, float height);
		static void SetNextWindowPos(const ImVec2& pos, ImGuiCond cond = 0,
			const ImVec2& pivot = ImVec2(0, 0));
		static void SetNextWindowSize(const ImVec2& size, ImGuiCond cond = 0);
		static void SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond = 0);
		static void SetWindowFocus(const char* name);
		static void SetNextWindowClass(const ImGuiWindowClass* window_class);
		static bool IsItemActive();


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
		static bool CreateMenuItem(const char* name, bool* open, const char* shortcut = "");
		static void OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0);
		static bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);
		static bool BeginPopupContextWindow(const char* str_id, ImGuiMouseButton mb, bool over_items);
		static void EndPopup();
		static bool BeginPopupModal(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
		static void CloseCurrentPopup();
		static void SetColumns(int count = 1, const char* id = NULL, bool border = true);
		static void SetColumnOffset(int column_index, float offset_x);
		static void NextColumn();
		static void SetNextItemOpen(bool is_open, ImGuiCond cond = 0);
		static ImGuiStyle& GetStyle();


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

		template<typename T>
		static void DrawTextWrappedWidget(const char* varname, const T&)
		{
			ImGuiAPI::TextWrapped(varname);
		}

		template<typename T>
		static void TextColored(const ImVec4& col, const char* varname, const T& data)
		{
			ImGuiAPI::TextColored(col,varname,data);
		}

		static void TextUnformatted(const char* text, const char* text_end = NULL);

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

		// Input Controls
		static bool IsKeyPressed(int KeyIndex, bool repeat = true);
		static int GetKeyIndex(ImGuiKey key);
		static bool IsMouseDown(ImGuiMouseButton button);

		// Misc
		static bool CheckBoxBool(const char* name, bool* var, bool hideName = true);
		static bool ButtonBool(const char* name, const ImVec2& size = ImVec2(0, 0));
		static bool SmallButton(const char* label);
		static bool ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1),
			int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));


		/*************************************************************************/
		/*                            DRAG & DROP                                */
		/*************************************************************************/
		static bool BeginDragDropSource();
		static void EndDragDropSource();
		static void SetDragDropPayload(const char* type, 
			const void* data, size_t sz, ImGuiCond cond = 0);

		static bool BeginDragDropTarget();
		static void EndDragDropTarget();
		static const ImGuiPayload* AcceptDragDropPayload(const char* type, 
			ImGuiDragDropFlags flags = 0);

		/*************************************************************************/
		/*                           Utilities                                   */
		/*************************************************************************/
		// Mostly used together with the static widgets
		static void InsertSameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
		static void InsertHorizontalLineSeperator();
		static void PushItemWidth(float value);
		static bool IsItemHovered();
		static void SetToolTip(const char* message);
		static void NewLine();
		static void BeginToolTip();
		static void EndTooltip();
		static void PopTextWrapPos();
		static void PushTextWrapPos(float wrap_pos_x);
		static void SetScrollY(float scroll_y);
		static float GetFontSize();
		static void PushStyleColor(ImGuiCol idx, const ImVec4& col);
		static void PushStyleColor(ImGuiCol idx, ImU32 col);
		static void PopStyleColor(int count = 1);
		static bool IsMouseDoubleClicked(ImGuiMouseButton button);
		static bool IsItemClicked(ImGuiMouseButton mouse_button);
		static void PushID(const char* str_id);
		static void PushID(const char* str_id_begin, const char* str_id_end);
		static void PushID(const void* ptr_id);
		static void PushID(int int_id);
		static void PopID();
		static void Dummy(const ImVec2& size);
		static bool ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);


		/*************************************************************************/
		/*                         Visual Widgets                                */
		/*************************************************************************/
		static void Image(ImTextureID user_texture_id, const ImVec2& size, 
			const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), 
			const ImVec4& tint_col = ImVec4(1, 1, 1, 1), 
			const ImVec4& border_col = ImVec4(0, 0, 0, 0));
		static void PlotHistogram(const char* label, std::vector<float> values, 
			int values_offset = 0, const char* overlay_text = NULL, 
			float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), 
			int stride = sizeof(float));
	};
}
