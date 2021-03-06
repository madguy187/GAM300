#include "pch.h"
#include "ECGui.h"

namespace Eclipse
{
	bool ECGui::BeginMainWindow(const char* name)
	{
		return ImGuiAPI::BeginMainWindow(name);
	}

	bool ECGui::BeginMainWindowWithFlag(const char* name, bool* p_open, 
		ImGuiWindowFlags flags)
	{
		return ImGuiAPI::BeginMainWindowWithFlag(name, p_open, flags);
	}

	void ECGui::EndMainWindow()
	{
		ImGuiAPI::EndMainWindow();
	}

	bool ECGui::BeginChildWindow(ChildSettings settings)
	{
		return ImGuiAPI::BeginChildWindow(settings.Name, settings.Size,
			settings.HaveBorder, settings.flags);
	}

	void ECGui::EndChildWindow()
	{
		ImGuiAPI::EndChildWindow();
	}

	ImVec2 ECGui::GetWindowSize()
	{
		return ImGuiAPI::GetWindowSize();
	}

	ECVec2 ECGui::GetWindowPos()
	{
		ECVec2 pos{ ImGuiAPI::GetWindowPos().x,  ImGuiAPI::GetWindowPos().y };
		return pos;
	}

	ECVec2 ECGui::GetCursorScreenPos()
	{
		ECVec2 pos{ ImGuiAPI::GetCursorScreenPos().x,  ImGuiAPI::GetCursorScreenPos().y };
		return pos;
	}

	ImVec2 ECGui::GetContentRegionAvail()
	{
		return 	ImGuiAPI::GetContentRegionAvail();
	}

	float ECGui::GetWindowHeight()
	{
		return ImGuiAPI::GetWindowHeight();
	}

	float ECGui::GetWindowWidth()
	{
		return ImGuiAPI::GetWindowWidth();
	}

	void ECGui::SetWindowSize(float width, float height)
	{
		ImGuiAPI::SetWindowSize(width, height);
	}

	void ECGui::SetNextWindowPos(const ImVec2& pos, ImGuiCond cond, const ImVec2& pivot)
	{
		ImGuiAPI::SetNextWindowPos(pos, cond, pivot);
	}

	void ECGui::SetNextWindowSize(const ImVec2& size, ImGuiCond cond)
	{
		ImGui::SetNextWindowSize(size, cond);
	}

	void ECGui::SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond)
	{
		ImGuiAPI::SetNextWindowDockID(dock_id, cond);
	}

	void ECGui::SetWindowFocus(const char* name)
	{
		ImGuiAPI::SetWindowFocus(name);
	}

	void ECGui::SetNextWindowClass(const ImGuiWindowClass* window_class)
	{
		ImGuiAPI::SetNextWindowClass(window_class);
	}

	bool ECGui::IsItemActive()
	{
		return ImGuiAPI::IsItemActive();
	}

	void ECGui::SetColumns(int count, const char* id, bool border)
	{
		ImGuiAPI::SetColumns(count, id, border);
	}

	void ECGui::SetColumnOffset(int column_index, float offset_x)
	{
		ImGuiAPI::SetColumnOffset(column_index, offset_x);
	}

	void ECGui::NextColumn()
	{
		ImGuiAPI::NextColumn();
	}

	void ECGui::SetNextItemOpen(bool is_open, ImGuiCond cond)
	{
		ImGuiAPI::SetNextItemOpen(is_open, cond);
	}

	ImGuiStyle& ECGui::GetStyle()
	{
		return ImGuiAPI::GetStyle();
	}

	ImGuiViewport* ECGui::GetMainViewport()
	{
		return ImGuiAPI::GetMainViewport();
	}

	ImDrawList* ECGui::GetWindowDrawList()
	{
		return ImGuiAPI::GetWindowDrawList();
	}

	ImVec2 ECGui::GetItemRectMin()
	{
		return ImGuiAPI::GetItemRectMin();
	}

	ImVec2 ECGui::GetItemRectMax()
	{
		return ImGuiAPI::GetItemRectMax();
	}

	bool ECGui::BeginPopUpButtonList(PopUpButtonSettings settings)
	{
		return ImGuiAPI::BeginPopUpButtonList(settings.Name, settings.ID);
	}

	void ECGui::EndPopUpButtonList()
	{
		ImGuiAPI::EndPopUpButtonList();
	}

	bool ECGui::BeginMenuBar()
	{
		return ImGuiAPI::BeginMenuBar();
	}

	void ECGui::EndMenuBar()
	{
		ImGuiAPI::EndMenuBar();
	}

	bool ECGui::BeginMenuComponents(const char* name)
	{
		return ImGuiAPI::BeginMenuComponents(name);
	}

	void ECGui::EndMenuComponents()
	{
		ImGuiAPI::EndMenuComponents();
	}

	void ECGui::BeginStyleVariant(ImGuiStyleVar id, float scale)
	{
		ImGuiAPI::BeginStyleVariant(id, scale);
	}

	void ECGui::EndStyleVariant()
	{
		ImGuiAPI::EndStyleVariant();
	}

	bool ECGui::CreateComboList(ComboListSettings settings, const std::vector<std::string>& vecStr, size_t& index)
	{
		// Commented out for Now - Darren
		//ENGINE_LOG_ASSERT(index >= vecStr.size(), "Accessing out of the vector's index");
		const char* combo_label = vecStr[index].c_str();
		if (ImGuiAPI::BeginComboList(settings.Name, combo_label, settings.HideName, settings.flags))
		{
			for (size_t n = 0; n < vecStr.size(); n++)
			{
				const bool is_selected = (index == n);

				if (ImGui::Selectable(vecStr[n].c_str(), is_selected))
				{
					index = n;
					ImGuiAPI::EndComboList();
					return true;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGuiAPI::EndComboList();
		}

		return false;
	}

	bool ECGui::CreateMenuItem(const char* name, bool* open, const char* shortcut)
	{
		return ImGuiAPI::CreateMenuItem(name, open, shortcut);
	}

	void ECGui::OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags)
	{
		ImGuiAPI::OpenPopup(str_id, popup_flags);
	}

	bool ECGui::BeginPopup(const char* str_id, ImGuiWindowFlags flags)
	{
		return ImGuiAPI::BeginPopup(str_id, flags);
	}

	bool ECGui::BeginPopupContextWindow(const char* str_id, ImGuiMouseButton mb, bool over_items)
	{
		return ImGuiAPI::BeginPopupContextWindow(str_id, mb, over_items);
	}

	void ECGui::EndPopup()
	{
		ImGuiAPI::EndPopup();
	}

	bool ECGui::BeginPopupModal(const char* name, bool* p_open, ImGuiWindowFlags flags)
	{
		return ImGuiAPI::BeginPopupModal(name, p_open, flags);
	}

	void ECGui::CloseCurrentPopup()
	{
		ImGuiAPI::CloseCurrentPopup();
	}

	bool ECGui::BeginComboList(const char* name, const char* previewCurrent, bool hideName, ImGuiComboFlags flags)
	{
		return ImGuiAPI::BeginComboList(name, previewCurrent, hideName, flags);
	}

	void ECGui::EndComboList()
	{
		ImGuiAPI::EndComboList();
	}

	bool ECGui::DrawDataHeader(const char* varname, ImGuiTextFilter& filter)
	{
		if (filter.PassFilter(varname))
		{
			ImGuiAPI::Text(varname);
			return true;
		}

		return false;
	}

	bool ECGui::CreateCollapsingHeader(const char* name, bool forInspector)
	{
		(void)forInspector;
		return ImGuiAPI::CreateCollapsingHeader(name);
	}

	bool ECGui::CreateSelectableButton(const char* label, bool* active, ImGuiSelectableFlags flags)
	{
		return ImGuiAPI::CreateSelectableButton(label, active, flags);
	}

	bool ECGui::BeginTreeNode(const char* name)
	{
		return ImGuiAPI::BeginTreeNode(name);
	}

	void ECGui::EndTreeNode()
	{
		ImGuiAPI::EndTreeNode();
	}

	void ECGui::TextUnformatted(const char* text, const char* text_end)
	{
		ImGui::TextUnformatted(text, text_end);
	}

	bool ECGui::DrawSliderIntWidget(const char* name, int* var, bool hideName, int minrange, int maxrange)
	{
		if (ImGuiAPI::SliderInt(name, var, hideName, minrange, maxrange))
		{
			CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<int>{ *var, *var });
			return true;
		}
		else if (ImGui::IsItemDeactivatedAfterChange())
		{
			CommandHistory::DisableMergeForMostRecentCommand();
		}

		return false;
	}

	bool ECGui::DrawSliderFloatWidget(const char* name, float* var, bool hideName, float minrange, float maxrange)
	{
		if (ImGuiAPI::SliderFloat(name, var, hideName, minrange, maxrange))
		{
			CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<float>{ *var, *var });
			return true;
		}
		else if (ImGui::IsItemDeactivatedAfterChange())
		{
			CommandHistory::DisableMergeForMostRecentCommand();
		}

		return false;
	}

	bool ECGui::DrawSliderFloat2Widget(const char* name, ECVec2* vector, bool hideName, float minrange, float maxrange)
	{
		if (ImGuiAPI::SliderFloat2(name, (float*)vector, hideName, minrange, maxrange))
		{
			CommandHistory::RegisterCommand(new ECVec2DeltaCommand{ *vector, *vector });
			return true;
		}
		else if (ImGui::IsItemDeactivatedAfterChange())
		{
			CommandHistory::DisableMergeForMostRecentCommand();
		}

		return false;
	}

	bool ECGui::DrawSliderFloat3Widget(const char* name, ECVec3* vector, bool hideName, float minrange, float maxrange,
		Entity ID)
	{
		if (ImGuiAPI::SliderFloat3(name, (float*)vector, hideName, minrange, maxrange))
		{
			CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ *vector, *vector, ID});
			return true;
		}
		else if (ImGui::IsItemDeactivatedAfterChange())
		{
			CommandHistory::DisableMergeForMostRecentCommand();
		}

		return false;
	}

	bool ECGui::DrawSliderFloat4Widget(const char* name, ECVec4* vector, bool hideName, float minrange, float maxrange)
	{
		if (ImGuiAPI::SliderFloat4(name, (float*)vector, hideName, minrange, maxrange))
		{
			CommandHistory::RegisterCommand(new ECVec4DeltaCommand{ *vector, *vector });
			return true;
		}
		else if (ImGui::IsItemDeactivatedAfterChange())
		{
			CommandHistory::DisableMergeForMostRecentCommand();
		}

		return false;
	}

	bool ECGui::DrawInputIntWidget(const char* name, int* var, bool hideName, int snapValue)
	{
		int oldValue = *var;

		if (ImGuiAPI::InputInt(name, var, hideName, snapValue))
		{
			CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<int>{ oldValue, *var });
			return true;
		}

		return false;
	}

	bool ECGui::DrawInputFloatWidget(const char* name, float* var, bool hideName, float snapValue)
	{
		float oldValue = *var;

		if (ImGuiAPI::InputFloat(name, var, hideName, snapValue))
		{
			CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<float>{ oldValue, *var  });
			return true;
		}

		return false;
	}

	bool ECGui::DrawInputFloat2Widget(const char* name, ECVec2* vector, bool hideName)
	{
		if (ImGuiAPI::InputFloat2(name, (float*)vector, hideName))
		{
			CommandHistory::RegisterCommand(new ECVec2DeltaCommand{ *vector, *vector });
			return true;
		}
		else if (ImGui::IsItemDeactivatedAfterChange())
		{
			CommandHistory::DisableMergeForMostRecentCommand();
		}

		return false;
	}

	bool ECGui::DrawInputFloat3Widget(const char* name, ECVec3* vector, bool hideName, Entity ID)
	{
		if (ImGuiAPI::InputFloat3(name, (float*)vector, hideName))
		{
			CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ *vector, *vector, ID });
			return true;
		}
		else if (ImGui::IsItemDeactivatedAfterChange())
		{
			CommandHistory::DisableMergeForMostRecentCommand();
		}

		return false;
	}

	bool ECGui::DrawInputFloat4Widget(const char* name, ECVec4* vector, bool hideName)
	{
		if (ImGuiAPI::InputFloat4(name, (float*)vector, hideName))
		{
			CommandHistory::RegisterCommand(new ECVec4DeltaCommand{ *vector, *vector });
			return true;
		}
		else if (ImGui::IsItemDeactivatedAfterChange())
		{
			CommandHistory::DisableMergeForMostRecentCommand();
		}

		return false;
	}

	bool ECGui::DrawInputTextHintWidget(const char* name, const char* hintText, char* buffer, size_t bufferSize, 
		bool hideName, ImGuiInputTextFlags flags)
	{
		if (ImGuiAPI::InputTextWithHint(name, hintText, buffer, bufferSize, hideName, flags))
		{
			CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<char*>{ buffer, buffer });
			return true;
		}
		else if (ImGui::IsItemDeactivatedAfterChange())
		{
			CommandHistory::DisableMergeForMostRecentCommand();
		}

		return false;
	}

	bool ECGui::DrawInputTextWidget(const char* name, char* buffer, size_t bufferSize, ImGuiInputTextFlags flag, bool hideName)
	{
		return ImGuiAPI::InputText(name, buffer, bufferSize, flag, hideName);
	}

	bool ECGui::IsKeyPressed(int KeyIndex, bool repeat)
	{
		return ImGuiAPI::IsKeyPressed(KeyIndex, repeat);
	}

	int ECGui::GetKeyIndex(ImGuiKey key)
	{
		return ImGuiAPI::GetKeyIndex(key);
	}

	bool ECGui::IsKeyDown(int user_key_index)
	{
		return ImGuiAPI::IsKeyDown(user_key_index);
	}

	bool ECGui::IsMouseDown(ImGuiMouseButton button)
	{
		return ImGuiAPI::IsMouseDown(button);
	}

	bool ECGui::IsMouseClicked(ImGuiMouseButton button, bool repeat)
	{
		return ImGuiAPI::IsMouseClicked(button, repeat);
	}

	bool ECGui::IsMouseDragging(ImGuiMouseButton button, float lock_threshold)
	{
		return ImGuiAPI::IsMouseDragging(button, lock_threshold);
	}

	ImVec2 ECGui::GetMouseDragDelta(ImGuiMouseButton button, float lock_threshold)
	{
		return ImGuiAPI::GetMouseDragDelta(button, lock_threshold);
	}

	bool ECGui::CheckBoxBool(const char* name, bool* var, bool hideName)
	{
		bool oldBool = *var;

		if (ImGuiAPI::CheckBoxBool(name, var, hideName))
		{
			CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<bool>{ oldBool, *var });
			return true;
		}

		return false;
	}

	bool ECGui::ButtonBool(const char* name, const ImVec2& size)
	{
		return ImGuiAPI::ButtonBool(name, size);
	}

	bool ECGui::SmallButton(const char* label)
	{
		return ImGuiAPI::SmallButton(label);
	}

	bool ECGui::ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
	{
		return ImGuiAPI::ImageButton(user_texture_id, size, uv0, uv1, frame_padding, bg_col, tint_col);
	}

	bool ECGui::BeginDragDropSource()
	{
		return ImGuiAPI::BeginDragDropSource();
	}

	void ECGui::EndDragDropSource()
	{
		ImGuiAPI::EndDragDropSource();
	}

	void ECGui::SetDragDropPayload(const char* type, const void* data, size_t sz, ImGuiCond cond)
	{
		ImGuiAPI::SetDragDropPayload(type, data, sz, cond);
	}

	bool ECGui::BeginDragDropTarget()
	{
		return ImGuiAPI::BeginDragDropTarget();
	}

	void ECGui::EndDragDropTarget()
	{
		ImGuiAPI::EndDragDropTarget();
	}

	const ImGuiPayload* ECGui::AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags)
	{
		return ImGuiAPI::AcceptDragDropPayload(type, flags);
	}

	void ECGui::InsertSameLine(float offset_from_start_x, float spacing)
	{
		ImGuiAPI::InsertSameLine(offset_from_start_x, spacing);
	}

	void ECGui::InsertHorizontalLineSeperator()
	{
		ImGuiAPI::InsertHorizontalLineSeperator();
	}

	void ECGui::PushItemWidth(float value)
	{
		ImGuiAPI::PushItemWidth(value);
	}

	bool ECGui::IsItemHovered()
	{
		return ImGuiAPI::IsItemHovered();
	}

	void ECGui::SetToolTip(const char* message)
	{
		ImGuiAPI::SetToolTip(message);
	}

	void ECGui::NewLine()
	{
		ImGuiAPI::NewLine();
	}

	void ECGui::BeginToolTip()
	{
		ImGuiAPI::BeginToolTip();
	}

	void ECGui::EndTooltip()
	{
		ImGuiAPI::EndTooltip();
	}

	void ECGui::PopTextWrapPos()
	{
		ImGuiAPI::PopTextWrapPos();
	}

	void ECGui::PushTextWrapPos(float wrap_pos_x)
	{
		ImGuiAPI::PushTextWrapPos(wrap_pos_x);
	}

	void ECGui::SetScrollY(float scroll_y)
	{
		ImGuiAPI::SetScrollY(scroll_y);
	}

	float ECGui::GetFontSize()
	{
		return ImGuiAPI::GetFontSize();
	}

	void ECGui::PushStyleColor(ImGuiCol idx, const ImVec4& col)
	{
		ImGuiAPI::PushStyleColor(idx, col);
	}

	void ECGui::PushStyleColor(ImGuiCol idx, ImU32 col)
	{
		ImGuiAPI::PushStyleColor(idx, col);
	}

	void ECGui::PopStyleColor(int count)
	{
		ImGuiAPI::PopStyleColor(count);
	}

	bool ECGui::IsMouseDoubleClicked(ImGuiMouseButton button)
	{
		return ImGuiAPI::IsMouseDoubleClicked(button);
	}

	bool ECGui::IsItemClicked(ImGuiMouseButton mouse_button)
	{
		return ImGui::IsItemClicked(mouse_button);
	}

	void ECGui::PushID(const char* str_id)
	{
		ImGuiAPI::PushID(str_id);
	}

	void ECGui::PushID(const char* str_id_begin, const char* str_id_end)
	{
		ImGuiAPI::PushID(str_id_begin, str_id_end);
	}

	void ECGui::PushID(const void* ptr_id)
	{
		ImGuiAPI::PushID(ptr_id);
	}

	void ECGui::PushID(int int_id)
	{
		ImGuiAPI::PushID(int_id);
	}

	void ECGui::PopID()
	{
		ImGuiAPI::PopID();
	}

	void ECGui::Dummy(const ImVec2& size)
	{
		ImGuiAPI::Dummy(size);
	}

	bool ECGui::ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags)
	{
		return ImGuiAPI::ColorPicker3(label, col, flags);
	}

	void ECGui::LineAndDummy(ImVec2 sizeOfDummies)
	{
		ECGui::InsertHorizontalLineSeperator();

		ECGui::Dummy(sizeOfDummies);
	}

	void ECGui::DummyAndLine(ImVec2 sizeOfDummies)
	{
		ECGui::Dummy(sizeOfDummies);

		ECGui::InsertHorizontalLineSeperator();
	}

	void ECGui::Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, 
		const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
	{
		ImGuiAPI::Image(user_texture_id, size, uv0, uv1, tint_col, border_col);
	}

	void ECGui::PlotHistogram(const char* name, std::vector<float> value, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
	{
		float values[50] = {0};
		//static double refresh_time = 0.0;
		std::copy(value.begin(), value.end(), values);
		engine->Timer.PrintCpuPercentage(values[0]);
		ImGui::PlotLines(name, values, IM_ARRAYSIZE(values), values_offset, overlay_text, scale_min, scale_max, graph_size,stride);
	}
}
