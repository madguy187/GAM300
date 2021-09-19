#include "pch.h"
#include "ECGui.h"

namespace Eclipse
{
	bool ECGui::BeginMainWindow(const char* name)
	{
		return ImGuiAPI::BeginMainWindow(name);
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

	void ECGui::CreateComboList(ComboListSettings settings, const std::vector<std::string>& vecStr, size_t& index)
	{
		const char* combo_label = vecStr[index].c_str();
		if (ImGuiAPI::BeginComboList(settings.Name, combo_label, settings.HideName, settings.flags))
		{
			for (size_t n = 0; n < vecStr.size(); n++)
			{
				const bool is_selected = (index == n);

				if (ImGui::Selectable(vecStr[n].c_str(), is_selected))
				{
					index = n;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGuiAPI::EndComboList();
		}
	}

	bool ECGui::CreateMenuItem(const char* name, bool* open)
	{
		return ImGuiAPI::CreateMenuItem(name, open);
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
		/*if (forInspector)
			engine->editorManager->DataComponentFilter_[name] = name;*/

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

	bool ECGui::DrawSliderFloat3Widget(const char* name, ECVec3* vector, bool hideName, float minrange, float maxrange)
	{
		if (ImGuiAPI::SliderFloat3(name, (float*)vector, hideName, minrange, maxrange))
		{
			CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ *vector, *vector });
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

	bool ECGui::DrawInputFloat3Widget(const char* name, ECVec3* vector, bool hideName)
	{
		if (ImGuiAPI::InputFloat3(name, (float*)vector, hideName))
		{
			CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ *vector, *vector });
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

	bool ECGui::CheckBoxBool(const char* name, bool* var, bool hideName)
	{
		return ImGuiAPI::CheckBoxBool(name, var, hideName);
	}

	bool ECGui::ButtonBool(const char* name)
	{
		return ImGuiAPI::ButtonBool(name);
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

	void ECGui::PlotHistogram(const char* name, std::vector<float> value, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
	{
		float values[100] = {0};
		//static double refresh_time = 0.0;
		std::copy(value.begin(), value.end(), values);
		//ImGui::PlotLines("Frame Times", values, IM_ARRAYSIZE(values));
		//ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
		ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%.2f %%", values[1]);
		//ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%.2f %%", values[1]);
		ImGui::PlotHistogram(name, values, IM_ARRAYSIZE(values), values_offset, overlay_text, scale_min, scale_max, graph_size,stride);
	}
}
