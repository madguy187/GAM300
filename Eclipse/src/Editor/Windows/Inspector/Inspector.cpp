#include "pch.h"
#include "Inspector.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
	void InspectorWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&InspectorWindow::DrawImpl, this));
	}

	InspectorWindow::InspectorWindow()
	{
		Type = EditorWindowType::INSPECTOR;
		WindowName = "Inspector";
	}

	void InspectorWindow::DrawImpl()
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
			ShowPointLightProperty(currEnt);
		}
		else
		{
			const char* entName = "No Entities";

			ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entName),
				strlen(entName), ImGuiInputTextFlags_ReadOnly);
		}

		ECGui::InsertHorizontalLineSeperator();
	}

	bool InspectorWindow::ShowEntityProperty(Entity ID)
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

	bool InspectorWindow::ShowTransformProperty(Entity ID)
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

	bool InspectorWindow::ShowPointLightProperty(Entity ID)
	{
		if (engine->world.CheckComponent<PointLightComponent>(ID))
		{
			if (ECGui::CreateCollapsingHeader("PointLight Component"))
			{
				auto& _PointLight = engine->world.GetComponent<PointLightComponent>(ID);

				ECGui::DrawTextWidget<const char*>("IntensityStrength", "");
				ECGui::DrawSliderFloatWidget("IntensityFloat", &_PointLight.IntensityStrength, true, 0.f, 150.f);

				ECGui::DrawTextWidget<const char*>("Light Colour", "");
				ECGui::DrawSliderFloat4Widget("ColourVec", &_PointLight.Color, true, 0.0f, 1.0f);
				engine->LightManager.SetLightColor(_PointLight, { _PointLight.Color.getX() ,_PointLight.Color.getY() , _PointLight.Color.getZ() , 1.0f });

				ECGui::DrawTextWidget<const char*>("Attenuation Level", "");
				ECGui::DrawSliderIntWidget("ColourVec", &_PointLight.AttenuationLevel, true, 0, 10);
				engine->LightManager.SetAttenuation(_PointLight, _PointLight.AttenuationLevel);

				ECGui::DrawTextWidget<const char*>("Light Ambient", "");
				ECGui::DrawSliderFloat3Widget("AmbientVec", &_PointLight.ambient, true, 0.0f, 1.0f);

				ECGui::DrawTextWidget<const char*>("Light Diffuse", "");
				ECGui::DrawSliderFloat3Widget("DiffuseVec", &_PointLight.diffuse, true, 0.0f, 1.0f);

				ECGui::DrawTextWidget<const char*>("Light Specular", "");
				ECGui::DrawSliderFloat3Widget("SpecularVec", &_PointLight.specular, true, 0.0f, 1.0f);
			}
		}

		return false;
	}

	bool InspectorWindow::ShowSpotLightProperty(Entity ID)
	{
		if (engine->world.CheckComponent<SpotLightComponent>(ID))
		{
			if (ECGui::CreateCollapsingHeader("PointLight Component"))
			{
				auto& _SpotLight = engine->world.GetComponent<SpotLightComponent>(ID);

			}
		}

		return false;
	}
}
