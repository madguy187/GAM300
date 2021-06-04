#include "pch.h"
#include "ECGui.h"

namespace Eclipse
{
	ImVec2 ECGui::GetWindowSize()
	{
		return instance.GetWindowSize();
	}

	void ECGui::SetWindowSize(float width, float height)
	{
		instance.SetWindowSize(width, height);
	}

	void ECGui::BeginStyleVariant(ImGuiStyleVar id, float scale)
	{
		instance.BeginStyleVariant(id, scale);
	}

	void ECGui::EndStyleVariant()
	{
		instance.EndStyleVariant();
	}

	void ECGui::CreateComboList(ComboListSettings settings, const std::vector<std::string>& vecStr, size_t& index)
	{
		const char* combo_label = vecStr[index].c_str();
		if (instance.BeginComboList(settings.Name, settings.CurrentPreview, settings.HideName, settings.flags))
		{
			for (size_t n = 0; n < vecStr.size(); n++)
			{
				const bool is_selected = (index == n);

				if (ImGui::Selectable(vecStr[n].c_str(), is_selected))
					index = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			instance.EndComboList();
		}
	}

	bool ECGui::CreateCollapsingHeader(const char* name)
	{
		return instance.CreateCollapsingHeader(name);
	}

	bool ECGui::CreateSelectableButton(const char* label, bool* active, ImGuiSelectableFlags flags)
	{
		return instance.CreateSelectableButton(label, active, flags);
	}

	bool ECGui::BeginTreeNode(const char* name)
	{
		return instance.BeginTreeNode(name);
	}

	void ECGui::EndTreeNode()
	{
		instance.EndTreeNode();
	}

	void ECGui::DrawSliderIntWidget(const char* name, int* var, bool hideName, int minrange, int maxrange)
	{
		instance.SliderInt(name, var, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderFloatWidget(const char* name, float* var, bool hideName, float minrange, float maxrange)
	{
		instance.SliderFloat(name, var, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderFloat2Widget(const char* name, float vector[2], bool hideName, float minrange, float maxrange)
	{
		instance.SliderFloat2(name, vector, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderFloat3Widget(const char* name, float vector[3], bool hideName, float minrange, float maxrange)
	{
		instance.SliderFloat3(name, vector, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderFloat4Widget(const char* name, float vector[4], bool hideName, float minrange, float maxrange)
	{
		instance.SliderFloat4(name, vector, hideName, minrange, maxrange);
	}

	void ECGui::DrawInputIntWidget(const char* name, int* var, bool hideName, int snapValue)
	{
		instance.InputInt(name, var, hideName, snapValue);
	}

	void ECGui::DrawInputFloatWidget(const char* name, float* var, bool hideName, float snapValue)
	{
		instance.InputFloat(name, var, hideName, snapValue);
	}

	void ECGui::DrawInputFloat2Widget(const char* name, float vector[2], bool hideName)
	{
		instance.InputFloat2(name, vector, hideName);
	}

	void ECGui::DrawInputFloat3Widget(const char* name, float vector[3], bool hideName)
	{
		instance.InputFloat3(name, vector, hideName);
	}

	void ECGui::DrawInputFloat4Widget(const char* name, float vector[4], bool hideName)
	{
		instance.InputFloat4(name, vector, hideName);
	}

	void ECGui::DrawInputTextHintWidget(const char* name, const char* hintText, char* buffer, size_t bufferSize, bool hideName)
	{
		instance.InputTextWithHint(name, hintText, buffer, bufferSize, hideName);
	}

	void ECGui::DrawInputTextWidget(const char* name, char* buffer, size_t bufferSize, bool hideName)
	{
		instance.InputText(name, buffer, bufferSize, hideName);
	}

	bool ECGui::CheckBoxBool(const char* name, bool* var, bool hideName)
	{
		return instance.CheckBoxBool(name, var, hideName);
	}

	bool ECGui::ButtonBool(const char* name)
	{
		return instance.ButtonBool(name);
	}

	void ECGui::InsertSameLine()
	{
		instance.InsertSameLine();
	}

	void ECGui::InsertHorizontalLineSeperator()
	{
		instance.InsertHorizontalLineSeperator();
	}

	void ECGui::PushItemWidth(float value)
	{
		instance.PushItemWidth(value);
	}

	bool ECGui::IsItemHovered()
	{
		return instance.IsItemHovered();
	}

	void ECGui::SetToolTip(const char* message)
	{
		instance.SetToolTip(message);
	}
}
