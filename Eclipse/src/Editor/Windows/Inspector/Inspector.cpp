#include "pch.h"
#include "Inspector.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
	void Inspector::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&Inspector::DrawImpl, this));
	}

	Inspector::Inspector()
	{
		Type = EditorWindowType::INSPECTOR;
		WindowName = "Inspector";
	}

	void Inspector::DrawImpl()
	{
		WindowSize_.setX(ECGui::GetWindowSize().x);
		WindowSize_.setY(ECGui::GetWindowSize().y);

		if (!engine->editorManager->EntityHierarchyList_.empty())
		{
			Entity currEnt = engine->editorManager->EntityHierarchyList_[engine->editorManager->GEHIndex_];
			auto& entcom = engine->world.GetComponent<EntityComponent>(currEnt);
			std::string entityName = entcom.Name + " " + std::to_string(currEnt);

			ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entityName.c_str()),
				entityName.size(), ImGuiInputTextFlags_ReadOnly);

			ECGui::InsertHorizontalLineSeperator();
			ECGui::PushItemWidth(WindowSize_.getX());

			ShowEntityProperty(currEnt);
			ShowTransformProperty(currEnt);
		}
		else
		{
			const char* entName = "No Entities";

			ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entName),
				strlen(entName), ImGuiInputTextFlags_ReadOnly);
		}

		ECGui::InsertHorizontalLineSeperator();
	}

	bool Inspector::ShowEntityProperty(Entity ID)
	{
		if (engine->world.CheckComponent<EntityComponent>(ID))
		{
			if (ECGui::CreateCollapsingHeader("Tag"))
			{
				static char entNameInput[256];
				auto& entCom = engine->world.GetComponent<EntityComponent>(ID);

				ECGui::DrawTextWidget<std::string>("Entity Tag", lexical_cast<std::string>(entCom.Tag));
				ECGui::DrawTextWidget<const char*>("Edit Name:", "");
				ECGui::InsertSameLine();
				ECGui::DrawInputTextHintWidget("InputEntityName", "Enter Entity Name", entNameInput, 256);

				if (ECGui::ButtonBool("Set Name"))
					entCom.Name = entNameInput;
			}
		}

		return false;
	}

	bool Inspector::ShowTransformProperty(Entity ID)
	{
		if (engine->world.CheckComponent<TransformComponent>(ID))
		{
			if (ECGui::CreateCollapsingHeader("Transform"))
			{
				auto& transCom = engine->world.GetComponent<TransformComponent>(ID);

				ECGui::DrawTextWidget<const char*>("Position", "");
				ECGui::DrawSliderFloat3Widget("TransVec", &transCom.position, true, -50.f, 50.f);

				ECGui::DrawTextWidget<const char*>("Rotation", "");
				ECGui::DrawSliderFloat3Widget("TransRot", &transCom.rotation, true, -360.f, 360.f);

				ECGui::DrawTextWidget<const char*>("Scale", "");
				ECGui::DrawSliderFloat3Widget("TransScale", &transCom.scale);
			}
		}

		return false;
	}
}
