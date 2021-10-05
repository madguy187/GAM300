#include "pch.h"
#include "ImGuiAPI.h"

#define HIDE_SYMBOL "##"

namespace Eclipse
{
	bool ImGuiAPI::BeginMainWindow(const char* name)
	{
		return ImGui::Begin(name);
	}

	bool ImGuiAPI::BeginMainWindowWithFlag(const char* name, bool* p_open,
		ImGuiWindowFlags flags)
	{
		return ImGui::Begin(name, p_open, flags);
	}

	void ImGuiAPI::EndMainWindow()
	{
		ImGui::End();
	}

	bool ImGuiAPI::BeginChildWindow(const char* name, ImVec2 size, bool border, ImGuiWindowFlags flag)
	{
		return ImGui::BeginChild(name, size, border, flag);
	}

	void ImGuiAPI::EndChildWindow()
	{
		return ImGui::EndChild();
	}

	ImVec2 ImGuiAPI::GetWindowSize()
	{
		return ImGui::GetContentRegionAvail();
	}

	ImVec2 ImGuiAPI::GetWindowPos()
	{
		return ImGui::GetWindowPos();
	}

	ImVec2 ImGuiAPI::GetCursorScreenPos()
	{
		return ImGui::GetCursorScreenPos();
	}

	ImVec2 ImGuiAPI::GetContentRegionAvail()
	{
		return ImGui::GetContentRegionAvail();
	}

	float ImGuiAPI::GetWindowHeight()
	{
		return ImGui::GetWindowHeight();
	}

	float ImGuiAPI::GetWindowWidth()
	{
		return ImGui::GetWindowWidth();
	}

	void ImGuiAPI::SetWindowSize(float width, float height)
	{
		ImGui::SetNextWindowSize(ImVec2(width, height));
	}

	void ImGuiAPI::SetNextWindowPos(const ImVec2& pos, ImGuiCond cond, const ImVec2& pivot)
	{
		ImGui::SetNextWindowPos(pos, cond, pivot);
	}

	void ImGuiAPI::SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond)
	{
		ImGui::SetNextWindowDockID(dock_id, cond);
	}

	void ImGuiAPI::SetWindowFocus(const char* name)
	{
		ImGui::SetWindowFocus(name);
	}

	void ImGuiAPI::SetNextWindowClass(const ImGuiWindowClass* window_class)
	{
		ImGui::SetNextWindowClass(window_class);
	}

	bool ImGuiAPI::IsItemActive()
	{
		return ImGui::IsItemActive();
	}

	void ImGuiAPI::SetColumns(int count, const char* id, bool border)
	{
		 ImGui::Columns(count, id, border);
	}

	void ImGuiAPI::SetColumnOffset(int column_index, float offset_x)
	{
		ImGui::SetColumnOffset(column_index, offset_x);
	}

	void ImGuiAPI::NextColumn()
	{
		ImGui::NextColumn();
	}

	void ImGuiAPI::SetNextItemOpen(bool is_open, ImGuiCond cond)
	{
		ImGui::SetNextItemOpen(is_open, cond);
	}

	ImGuiStyle& ImGuiAPI::GetStyle()
	{
		return ImGui::GetStyle();
	}

	ImGuiViewport* ImGuiAPI::GetMainViewport()
	{
		return  ImGui::GetMainViewport();
	}

	ImDrawList* ImGuiAPI::GetWindowDrawList()
	{
		return ImGui::GetWindowDrawList();
	}

	ImVec2 ImGuiAPI::GetItemRectMin()
	{
		return ImGui::GetItemRectMin();
	}

	ImVec2 ImGuiAPI::GetItemRectMax()
	{
		return ImGui::GetItemRectMax();
	}

	bool ImGuiAPI::BeginPopUpButtonList(const char* name, const char* id)
	{
		if (ButtonBool(name))
			ImGui::OpenPopup(id);

		return ImGui::BeginPopup(id);
	}

	void ImGuiAPI::EndPopUpButtonList()
	{
		ImGui::EndPopup();
	}

	void ImGuiAPI::BeginStyleVariant(ImGuiStyleVar id, float scale)
	{
		ImGui::PushStyleVar(id, scale);
	}

	void ImGuiAPI::EndStyleVariant()
	{
		ImGui::PopStyleVar();
	}

	bool ImGuiAPI::BeginMenuBar()
	{
		return ImGui::BeginMainMenuBar();
	}

	void ImGuiAPI::EndMenuBar()
	{
		ImGui::EndMainMenuBar();
	}

	bool ImGuiAPI::CreateMenuItem(const char* name, bool* open, const char* shortcut)
	{
		return ImGui::MenuItem(name, shortcut, open);
	}

	bool ImGuiAPI::BeginMenuComponents(const char* name)
	{
		return ImGui::BeginMenu(name);
	}

	void ImGuiAPI::EndMenuComponents()
	{
		ImGui::EndMenu();
	}

	bool ImGuiAPI::BeginComboList(const char* name, const char* previewCurrent, bool hideName, ImGuiComboFlags flags)
	{
		std::string finalID = HIDE_SYMBOL;

		if (hideName)
			finalID += name;
		else
			finalID = name;

		return ImGui::BeginCombo(finalID.c_str(), previewCurrent);
	}

	void ImGuiAPI::EndComboList()
	{
		ImGui::EndCombo();
	}

	bool ImGuiAPI::BeginTreeNode(const char* name)
	{
		return ImGui::TreeNode(name);
	}

	void ImGuiAPI::EndTreeNode()
	{
		ImGui::TreePop();
	}

	bool ImGuiAPI::BeginPopupContextWindow(const char* str_id, ImGuiMouseButton mb, bool over_items)
	{
		return ImGui::BeginPopupContextWindow(str_id, mb, over_items);
	}

	void ImGuiAPI::OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags)
	{
		ImGui::OpenPopup(str_id, popup_flags);
	}

	bool ImGuiAPI::BeginPopup(const char* str_id, ImGuiWindowFlags flags)
	{
		return ImGui::BeginPopup(str_id, flags);
	}

	void ImGuiAPI::EndPopup()
	{
		ImGui::EndPopup();
	}

	bool ImGuiAPI::BeginPopupModal(const char* name, bool* p_open, ImGuiWindowFlags flags)
	{
		return ImGui::BeginPopupModal(name, p_open, flags);
	}

	void ImGuiAPI::CloseCurrentPopup()
	{
		ImGui::CloseCurrentPopup();
	}

	bool ImGuiAPI::CreateCollapsingHeader(const char* name)
	{
		return ImGui::CollapsingHeader(name);
	}

	bool ImGuiAPI::CreateSelectableButton(const char* label, bool* active, ImGuiSelectableFlags flags)
	{
		return ImGui::Selectable(label, active, flags);
	}

	void ImGuiAPI::TextInt(const char* varname, const int& var)
	{
		ImGui::Text("%s: %d", varname, var);
	}

	void ImGuiAPI::TextFloat(const char* varname, const float& var)
	{
		ImGui::Text("%s: %.3f", varname, var);
	}

	void ImGuiAPI::TextDouble(const char* varname, const double& var)
	{
		ImGui::Text("%s: %.1f", varname, var);
	}

	void ImGuiAPI::TextSize_t(const char* varname, const size_t& var)
	{
		ImGui::Text("%s: %zu", varname, var);
	}

	void ImGuiAPI::TextBool(const char* varname, const bool& var)
	{
		ImGui::Text("%s: %s", varname, var ? "true" : "false");
	}

	void ImGuiAPI::TextString(const char* varname, const std::string& var)
	{
		ImGui::Text("%s: %s", varname, var.c_str());
	}

	void ImGuiAPI::TextStaticString(const char* varname, const char* var)
	{
		ImGui::Text("%s: %s", varname, var);
	}

	void ImGuiAPI::TextVec2(const char* varname, const ECVec2& var)
	{
		ImGui::Text("%s: {x{%.1f}, y{%.1f}}", varname, var.x, var.y);
	}

	void ImGuiAPI::TextVec3(const char* varname, const ECVec3& var)
	{
		ImGui::Text("%s: {x{%.1f}, y{%.1f}, z{%.1f}}", varname, var.x, var.y, var.z);
	}

	void ImGuiAPI::TextVec4(const char* varname, const ECVec4& var)
	{
		ImGui::Text("%s: {x{%.1f}, y{%.1f}, z{%.1f}, w{%.1f}}", varname, var.x, var.y, var.z, var.w);
	}

	void ImGuiAPI::TextMat3(const char* varname, const ECMat3& var)
	{
		ImGui::Text("%s: ", varname);

		for (size_t i = 0; i < var.Rows(); ++i)
		{
			for (size_t j = 0; j < var.Cols(); ++j)
			{
				if (j % var.Cols() != 0)
					InsertSameLine();

				ImGui::Text("%.3f ", var[i][j]);
			}
		}
	}

	void ImGuiAPI::TextMat4(const char* varname, const ECMat4& var)
	{
		ImGui::Text("%s: ", varname);

		for (size_t i = 0; i < var.Rows(); ++i)
		{
			for (size_t j = 0; j < var.Cols(); ++j)
			{
				if (j % var.Cols() != 0)
					InsertSameLine();

				ImGui::Text("%.3f ", var[i][j]);
			}
		}
	}

	void ImGuiAPI::Text(const char* varname)
	{
		ImGui::Text("%s ", varname);
	}

	void ImGuiAPI::TextWrapped(const char* varname)
	{
		ImGui::TextWrapped("%s ", varname);
	}


	bool ImGuiAPI::InputInt(const char* name, int* var, bool hideName, int snapValue)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::InputInt(finalID.c_str(), var, snapValue);
	}

	bool ImGuiAPI::InputFloat(const char* name, float* var, bool hideName, float snapValue)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::InputFloat(finalID.c_str(), var, snapValue);
	}

	bool ImGuiAPI::InputFloat2(const char* name, float vector[2], bool hideName)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::InputFloat2(finalID.c_str(), vector);
	}

	bool ImGuiAPI::InputFloat3(const char* name, float vector[3], bool hideName)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::InputFloat3(finalID.c_str(), vector);
	}

	bool ImGuiAPI::InputFloat4(const char* name, float vector[4], bool hideName)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::InputFloat4(finalID.c_str(), vector);
	}

	bool ImGuiAPI::InputTextWithHint(const char* name, const char* hintText, char* buffer, size_t bufferSize,
		bool hideName, ImGuiInputTextFlags flags)
	{
		//Text(name);
		std::string finalID = HideWidgetName(name, true);
		/*InsertSameLine();*/
		return ImGui::InputTextWithHint(finalID.c_str(), hintText, buffer, bufferSize, flags);
	}

	bool ImGuiAPI::InputText(const char* name, char* buffer, size_t bufferSize, ImGuiInputTextFlags flag, bool hideName)
	{
		//Text(name);
		std::string finalID = HideWidgetName(name, true);
		/*InsertSameLine();*/
		return ImGui::InputText(finalID.c_str(), buffer, bufferSize, flag);
	}

	bool ImGuiAPI::SliderInt(const char* name, int* var, bool hideName, int minrange, int maxrange)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::SliderInt(finalID.c_str(), var, minrange, maxrange);
	}

	bool ImGuiAPI::SliderFloat(const char* name, float* var, bool hideName, float minrange, float maxrange)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::SliderFloat(finalID.c_str(), var, minrange, maxrange);
	}

	bool ImGuiAPI::SliderFloat2(const char* name, float vector[2], bool hideName, float minrange, float maxrange)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::SliderFloat2(finalID.c_str(), vector, minrange, maxrange);
	}

	bool ImGuiAPI::SliderFloat3(const char* name, float vector[3], bool hideName, float minrange, float maxrange)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::SliderFloat3(finalID.c_str(), vector, minrange, maxrange);
	}

	bool ImGuiAPI::SliderFloat4(const char* name, float vector[4], bool hideName, float minrange, float maxrange)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::SliderFloat4(finalID.c_str(), vector, minrange, maxrange);
	}

	bool ImGuiAPI::IsKeyPressed(int KeyIndex, bool repeat)
	{
		return ImGui::IsKeyPressed(KeyIndex, repeat);
	}

	int ImGuiAPI::GetKeyIndex(ImGuiKey key)
	{
		return ImGui::GetKeyIndex(key);
	}

	bool ImGuiAPI::IsMouseDown(ImGuiMouseButton button)
	{
		return ImGui::IsMouseDown(button);
	}

	bool ImGuiAPI::CheckBoxBool(const char* name, bool* var, bool hideName)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::Checkbox(finalID.c_str(), var);
	}
	bool ImGuiAPI::ButtonBool(const char* name, const ImVec2& size)
	{
		return ImGui::Button(name, size);
	}

	bool ImGuiAPI::SmallButton(const char* label)
	{
		return ImGui::SmallButton(label);
	}

	bool ImGuiAPI::ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		return ImGui::ImageButton(user_texture_id, size, uv0, uv1, frame_padding, bg_col, tint_col);
	}

	bool ImGuiAPI::BeginDragDropSource()
	{
		return ImGui::BeginDragDropSource();
	}

	void ImGuiAPI::EndDragDropSource()
	{
		ImGui::EndDragDropSource();
	}

	void ImGuiAPI::SetDragDropPayload(const char* type, const void* data, size_t sz, ImGuiCond cond)
	{
		ImGui::SetDragDropPayload(type, data, sz, cond);
	}

	bool ImGuiAPI::BeginDragDropTarget()
	{
		return ImGui::BeginDragDropTarget();
	}

	void ImGuiAPI::EndDragDropTarget()
	{
		ImGui::EndDragDropTarget();
	}

	const ImGuiPayload* ImGuiAPI::AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags)
	{
		return ImGui::AcceptDragDropPayload(type, flags);
	}

	void ImGuiAPI::InsertSameLine(float offset_from_start_x, float spacing)
	{
		ImGui::SameLine(offset_from_start_x, spacing);
	}

	void ImGuiAPI::InsertHorizontalLineSeperator()
	{
		ImGui::Separator();
	}

	void ImGuiAPI::PushItemWidth(float value)
	{
		ImGui::PushItemWidth(value);
	}

	bool ImGuiAPI::IsItemHovered()
	{
		return ImGui::IsItemHovered();
	}

	void ImGuiAPI::SetToolTip(const char* message)
	{
		ImGui::SetTooltip(message);
	}

	void ImGuiAPI::HelpMarker(const char* message)
	{
		ImGui::PushStyleColor(ImGuiCol_TextDisabled, (ImVec4)ImColor::HSV(1, 1.f, 1.6f, 1.f));
		ImGui::TextDisabled("(?)");
		ImGui::PopStyleColor(1);
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(message);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void ImGuiAPI::NewLine()
	{
		ImGui::NewLine();
	}

	void ImGuiAPI::BeginToolTip()
	{
		ImGui::BeginTooltip();
	}

	void ImGuiAPI::EndTooltip()
	{
		ImGui::EndTooltip();
	}

	void ImGuiAPI::PopTextWrapPos()
	{
		ImGui::PopTextWrapPos();
	}

	void ImGuiAPI::PushTextWrapPos(float wrap_pos_x)
	{
		ImGui::PushTextWrapPos(wrap_pos_x);
	}

	void ImGuiAPI::SetScrollY(float scroll_y)
	{
		ImGui::SetScrollY(scroll_y);
	}

	float ImGuiAPI::GetFontSize()
	{
		return ImGui::GetFontSize();
	}

	void ImGuiAPI::PushStyleColor(ImGuiCol idx, const ImVec4& col)
	{
		ImGui::PushStyleColor(idx, col);
	}

	void ImGuiAPI::PushStyleColor(ImGuiCol idx, ImU32 col)
	{
		ImGui::PushStyleColor(idx,col);
	}

	void ImGuiAPI::PopStyleColor(int count)
	{
		ImGui::PopStyleColor(count);
	}

	void ImGuiAPI::Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
	{
		ImGui::Image(user_texture_id, size, uv0, uv1, tint_col, border_col);
	}

	bool ImGuiAPI::IsMouseDoubleClicked(ImGuiMouseButton button)
	{
		return ImGui::IsMouseDoubleClicked(button);
	}

	bool ImGuiAPI::IsItemClicked(ImGuiMouseButton mouse_button)
	{
		return ImGui::IsItemClicked(mouse_button);
	}

	void ImGuiAPI::PushID(const char* str_id)
	{
		ImGui::PushID(str_id);
	}

	void ImGuiAPI::PushID(const char* str_id_begin, const char* str_id_end)
	{
		ImGui::PushID(str_id_begin, str_id_end);
	}

	void ImGuiAPI::PushID(const void* ptr_id)
	{
		ImGui::PushID(ptr_id);
	}

	void ImGuiAPI::PushID(int int_id)
	{
		ImGui::PushID(int_id);
	}

	void ImGuiAPI::PopID()
	{
		ImGui::PopID();
	}

	bool ImGuiAPI::ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags)
	{
		return ImGui::ColorPicker3(label,col, flags);
	}

	void ImGuiAPI::Dummy(const ImVec2& size)
	{
		ImGui::Dummy(size);
	}

	void ImGuiAPI::PlotHistogram(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
	{
		ImGui::PlotHistogram(label, values, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size, stride);
	}

	std::string ImGuiAPI::HideWidgetName(const char* name, bool Changing)
	{
		std::string finalID = HIDE_SYMBOL;

		if (Changing)
			finalID += name;
		else
			finalID = name;

		return finalID;
	}
}
