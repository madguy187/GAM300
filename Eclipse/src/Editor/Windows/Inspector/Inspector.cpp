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
		Type = EditorWindowType::EWT_INSPECTOR;
		WindowName = "Inspector";
	}

	void InspectorWindow::DrawImpl()
	{
		WindowSize_.setX(ECGui::GetWindowSize().x);
		WindowSize_.setY(ECGui::GetWindowSize().y);

		if (!engine->editorManager->EntityHierarchyList_.empty())
		{
			Entity currEnt = engine->editorManager->GetSelectedEntity();
			auto& entcom = engine->world.GetComponent<EntityComponent>(currEnt);
			std::string entityName = entcom.Name + " " + std::to_string(currEnt);

			ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entityName.c_str()),
				entityName.size(), ImGuiInputTextFlags_ReadOnly);

			ECGui::InsertHorizontalLineSeperator();

			static ImGuiTextFilter CompFilter;
			CompFilter.Draw();

			ECGui::PushItemWidth(WindowSize_.getX());
			ShowEntityProperty("Tag", currEnt, CompFilter);
			ShowTransformProperty("Transform", currEnt, CompFilter);
			ShowPointLightProperty("PointLight", currEnt, CompFilter);
			ShowSpotLightProperty("SpotLight", currEnt, CompFilter);
			ShowDirectionalLightProperty("DirectionalLight", currEnt, CompFilter);
		}
		else
		{
			const char* entName = "No Entities";

			ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entName),
				strlen(entName), ImGuiInputTextFlags_ReadOnly);
		}

		ECGui::InsertHorizontalLineSeperator();
	}

	bool InspectorWindow::ShowEntityProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
	{
		if (engine->world.CheckComponent<EntityComponent>(ID))
		{
			if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
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

	bool InspectorWindow::ShowTransformProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
	{
		if (engine->world.CheckComponent<TransformComponent>(ID))
		{
			if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
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

	bool InspectorWindow::ShowPointLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
	{
		if (engine->world.CheckComponent<PointLightComponent>(ID))
		{
			if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
			{
				auto& _PointLight = engine->world.GetComponent<PointLightComponent>(ID);

				ECGui::DrawTextWidget<const char*>("IntensityStrength", "");
				ECGui::DrawSliderFloatWidget("IntensityFloat", &_PointLight.IntensityStrength, true, 0.f, 150.f);

				ECGui::DrawTextWidget<const char*>("Light Colour", "");
				ImGui::ColorPicker3("PLightColor", (float*)&_PointLight.Color, 
					ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
				//ECGui::DrawSliderFloat4Widget("ColourVec", &_PointLight.Color, true, 0.0f, 1.0f);
				engine->LightManager.SetLightColor(_PointLight, 
					{ _PointLight.Color.getX() ,_PointLight.Color.getY() , _PointLight.Color.getZ() , 1.0f });

				ECGui::DrawTextWidget<const char*>("Attenuation Level", "");
				ECGui::DrawSliderIntWidget("PLightColourVec", &_PointLight.AttenuationLevel, true, 0, 10);
				engine->LightManager.SetAttenuation(_PointLight, _PointLight.AttenuationLevel);

				ECGui::DrawTextWidget<const char*>("Light Ambient", "");
				ECGui::DrawSliderFloat3Widget("PLightAmbientVec", &_PointLight.ambient, true, 0.0f, 1.0f);

				ECGui::DrawTextWidget<const char*>("Light Diffuse", "");
				ECGui::DrawSliderFloat3Widget("PLightDiffuseVec", &_PointLight.diffuse, true, 0.0f, 1.0f);

				ECGui::DrawTextWidget<const char*>("Light Specular", "");
				ECGui::DrawSliderFloat3Widget("PLightSpecularVec", &_PointLight.specular, true, 0.0f, 1.0f);
			}
		}

		return false;
	}

	bool InspectorWindow::ShowSpotLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
	{
		if (engine->world.CheckComponent<SpotLightComponent>(ID))
		{
			if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
			{
				auto& _SpotLight = engine->world.GetComponent<SpotLightComponent>(ID);

				ECGui::DrawTextWidget<const char*>("IntensityStrength", "");
				ECGui::DrawSliderFloatWidget("IntensityFloat", &_SpotLight.IntensityStrength, true, 0.f, 150.f);

				ECGui::DrawTextWidget<const char*>("Light Colour", "");
				ImGui::ColorPicker3("SLightColor", (float*)&_SpotLight.lightColor, 
					ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);

				ECGui::DrawTextWidget<const char*>("OuterCutOff", "");
				ECGui::DrawSliderFloatWidget("SLightOuterCutOffFloat", &_SpotLight.outerCutOff, true, 0.f, 50.0f);

				ECGui::DrawTextWidget<const char*>("CutOff", "");
				ECGui::DrawSliderFloatWidget("SLightCutOffFloat", &_SpotLight.cutOff, true, 0.f, (_SpotLight.outerCutOff - 5.0f));

				ECGui::DrawTextWidget<const char*>("Direction", "");
				ECGui::DrawSliderFloat3Widget("SLightDirectionVec", &_SpotLight.direction, true, 0.f, 150.f);
			}
		}

		return false;
	}

	bool InspectorWindow::ShowDirectionalLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
	{
		if (engine->world.CheckComponent<DirectionalLightComponent>(ID))
		{
			if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
			{
				auto& _DLight = engine->world.GetComponent<DirectionalLightComponent>(ID);

				ECGui::DrawTextWidget<const char*>("Light Colour", "");
				ImGui::ColorPicker3("DLightColor", (float*)&_DLight.lightColor, 
					ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);

				ECGui::DrawTextWidget<const char*>("DLight Ambient", "");
				ECGui::DrawSliderFloat3Widget("DLightAmbientVec", &_DLight.ambient, true, 0.0f, 1.0f);

				ECGui::DrawTextWidget<const char*>("DLight Diffuse", "");
				ECGui::DrawSliderFloat3Widget("DLightDiffuseVec", &_DLight.diffuse, true, 0.0f, 1.0f);

				ECGui::DrawTextWidget<const char*>("DLight Specular", "");
				ECGui::DrawSliderFloat3Widget("DLightSpecularVec", &_DLight.specular, true, 0.0f, 1.0f);
			}
		}

		return false;
	}
}
