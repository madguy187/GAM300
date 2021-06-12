#include "pch.h"
#include "ImGuiAPI.h"

#define HIDE_SYMBOL "##"

namespace Eclipse
{
	bool ImGuiAPI::BeginMainWindow(const char* name)
	{
		return ImGui::Begin(name);
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

	void ImGuiAPI::SetWindowSize(float width, float height)
	{
		ImGui::SetNextWindowSize(ImVec2(width, height));
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

	bool ImGuiAPI::CreateMenuItem(const char* name, bool* open)
	{
		return ImGui::MenuItem(name, "", open);
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
		return ImGui::TreeNode(name);;
	}

	void ImGuiAPI::EndTreeNode()
	{
		ImGui::TreePop();
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

	void ImGuiAPI::InputInt(const char* name, int* var, bool hideName, int snapValue)
	{
		std::string finalID = HideWidgetName(name, hideName);
		ImGui::InputInt(finalID.c_str(), var, snapValue);
	}

	void ImGuiAPI::InputFloat(const char* name, float* var, bool hideName, float snapValue)
	{
		std::string finalID = HideWidgetName(name, hideName);
		ImGui::InputFloat(finalID.c_str(), var, snapValue);
	}

	void ImGuiAPI::InputFloat2(const char* name, float vector[2], bool hideName)
	{
		std::string finalID = HideWidgetName(name, hideName);
		ImGui::InputFloat2(finalID.c_str(), vector);
	}

	void ImGuiAPI::InputFloat3(const char* name, float vector[3], bool hideName)
	{
		std::string finalID = HideWidgetName(name, hideName);
		ImGui::InputFloat3(finalID.c_str(), vector);
	}

	void ImGuiAPI::InputFloat4(const char* name, float vector[4], bool hideName)
	{
		std::string finalID = HideWidgetName(name, hideName);
		ImGui::InputFloat4(finalID.c_str(), vector);
	}

	void ImGuiAPI::InputTextWithHint(const char* name, const char* hintText, char* buffer, size_t bufferSize, bool hideName)
	{
		//Text(name);
		std::string finalID = HideWidgetName(name, true);
		InsertSameLine();
		ImGui::InputTextWithHint(finalID.c_str(), hintText, buffer, bufferSize);
	}

	void ImGuiAPI::InputText(const char* name, char* buffer, size_t bufferSize, bool hideName)
	{
		//Text(name);
		std::string finalID = HideWidgetName(name, true);
		InsertSameLine();
		ImGui::InputText(finalID.c_str(), buffer, bufferSize);
	}

	void ImGuiAPI::SliderInt(const char* name, int* var, bool hideName, int minrange, int maxrange)
	{
		std::string finalID = HideWidgetName(name, hideName);
		ImGui::SliderInt(finalID.c_str(), var, minrange, maxrange);
	}

	void ImGuiAPI::SliderFloat(const char* name, float* var, bool hideName, float minrange, float maxrange)
	{
		std::string finalID = HideWidgetName(name, hideName);
		ImGui::SliderFloat(finalID.c_str(), var, minrange, maxrange);
	}

	void ImGuiAPI::SliderFloat2(const char* name, float vector[2], bool hideName, float minrange, float maxrange)
	{
		std::string finalID = HideWidgetName(name, hideName);
		ImGui::SliderFloat2(finalID.c_str(), vector, minrange, maxrange);
	}

	void ImGuiAPI::SliderFloat3(const char* name, float vector[3], bool hideName, float minrange, float maxrange)
	{
		std::string finalID = HideWidgetName(name, hideName);
		ImGui::SliderFloat3(finalID.c_str(), vector, minrange, maxrange);
	}

	void ImGuiAPI::SliderFloat4(const char* name, float vector[4], bool hideName, float minrange, float maxrange)
	{
		std::string finalID = HideWidgetName(name, hideName);
		ImGui::SliderFloat4(finalID.c_str(), vector, minrange, maxrange);
	}

	bool ImGuiAPI::CheckBoxBool(const char* name, bool* var, bool hideName)
	{
		std::string finalID = HideWidgetName(name, hideName);
		return ImGui::Checkbox(finalID.c_str(), var);
	}
	bool ImGuiAPI::ButtonBool(const char* name)
	{
		return ImGui::Button(name);
	}

	void ImGuiAPI::InsertSameLine()
	{
		ImGui::SameLine();
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
