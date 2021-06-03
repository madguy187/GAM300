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

	bool ECGui::BeginComboList(const char* name, const char* previewCurrent, bool hideName, ImGuiComboFlags flags)
	{
		return false;
	}

	void ECGui::EndComboList()
	{
		instance.EndComboList();
	}

	bool ECGui::CreateCollapsingHeader(const char* name)
	{
		return instance.CreateCollapsingHeader(name);
	}

	void ECGui::DrawSliderWidget(const char* name, int* var, bool hideName, int minrange, int maxrange)
	{
		instance.SliderInt(name, var, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderWidget(const char* name, float* var, bool hideName, float minrange, float maxrange)
	{
		instance.SliderFloat(name, var, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderWidget(const char* name, float vector[2], bool hideName, float minrange, float maxrange)
	{
		instance.SliderFloat2(name, vector, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderWidget(const char* name, float vector[3], bool hideName, float minrange, float maxrange)
	{
		instance.SliderFloat3(name, vector, hideName, minrange, maxrange);
	}

	void ECGui::DrawSliderWidget(const char* name, float vector[4], bool hideName, float minrange, float maxrange)
	{
		instance.SliderFloat4(name, vector, hideName, minrange, maxrange);
	}

	void ECGui::DrawInputWidget(const char* name, int* var, bool hideName, int snapValue)
	{
		instance.InputInt(name, var, hideName, snapValue);
	}

	void ECGui::DrawInputWidget(const char* name, float* var, bool hideName, float snapValue)
	{
		instance.InputFloat(name, var, hideName, snapValue);
	}

	void ECGui::DrawInputWidget(const char* name, float vector[2], bool hideName)
	{
		instance.InputFloat2(name, vector, hideName);
	}

	void ECGui::DrawInputWidget(const char* name, float vector[3], bool hideName)
	{
		instance.InputFloat3(name, vector, hideName);
	}

	void ECGui::DrawInputWidget(const char* name, float vector[4], bool hideName)
	{
		instance.InputFloat4(name, vector, hideName);
	}

	void ECGui::DrawInputWidget(const char* name, const char* hintText, char* buffer, size_t bufferSize, bool hideName)
	{
		instance.InputTextWithHint(name, hintText, buffer, bufferSize, hideName);
	}

	void ECGui::DrawInputWidget(const char* name, char* buffer, size_t bufferSize, bool hideName)
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
}
