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
		if (ImGuiAPI::BeginComboList(settings.Name, settings.CurrentPreview, settings.HideName, settings.flags))
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

	bool ECGui::CreateCollapsingHeader(const char* name)
	{
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

	void ECGui::DrawSliderIntWidget(const char* name, int* var, bool hideName, int minrange, int maxrange)
	{
		ImGuiAPI::SliderInt(name, var, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderFloatWidget(const char* name, float* var, bool hideName, float minrange, float maxrange)
	{
		ImGuiAPI::SliderFloat(name, var, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderFloat2Widget(const char* name, ECVec2* vector, bool hideName, float minrange, float maxrange)
	{
		ImGuiAPI::SliderFloat2(name, (float*)vector, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderFloat3Widget(const char* name, ECVec3* vector, bool hideName, float minrange, float maxrange)
	{
		ImGuiAPI::SliderFloat3(name, (float*)vector, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderFloat4Widget(const char* name, ECVec4* vector, bool hideName, float minrange, float maxrange)
	{
		ImGuiAPI::SliderFloat4(name, (float*)vector, hideName, minrange, maxrange);
	}

	void ECGui::DrawInputIntWidget(const char* name, int* var, bool hideName, int snapValue)
	{
		ImGuiAPI::InputInt(name, var, hideName, snapValue);
	}

	void ECGui::DrawInputFloatWidget(const char* name, float* var, bool hideName, float snapValue)
	{
		ImGuiAPI::InputFloat(name, var, hideName, snapValue);
	}

	void ECGui::DrawInputFloat2Widget(const char* name, ECVec2* vector, bool hideName)
	{
		ImGuiAPI::InputFloat2(name, (float*)vector, hideName);
	}

	void ECGui::DrawInputFloat3Widget(const char* name, ECVec3* vector, bool hideName)
	{
		ImGuiAPI::InputFloat3(name, (float*)vector, hideName);
	}

	void ECGui::DrawInputFloat4Widget(const char* name, ECVec4* vector, bool hideName)
	{
		ImGuiAPI::InputFloat4(name, (float*)vector, hideName);
	}

	void ECGui::DrawInputTextHintWidget(const char* name, const char* hintText, char* buffer, size_t bufferSize, bool hideName)
	{
		ImGuiAPI::InputTextWithHint(name, hintText, buffer, bufferSize, hideName);
	}

	void ECGui::DrawInputTextWidget(const char* name, char* buffer, size_t bufferSize, bool hideName)
	{
		ImGuiAPI::InputText(name, buffer, bufferSize, hideName);
	}

	bool ECGui::CheckBoxBool(const char* name, bool* var, bool hideName)
	{
		return ImGuiAPI::CheckBoxBool(name, var, hideName);
	}

	bool ECGui::ButtonBool(const char* name)
	{
		return ImGuiAPI::ButtonBool(name);
	}

	void ECGui::InsertSameLine()
	{
		ImGuiAPI::InsertSameLine();
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
}
