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

	void InspectorWindow::Unload()
	{
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

		if (!engine->editorManager->IsEntityListEmpty())
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
			//tianyu testing will delete later
			//todo
			if (engine->world.CheckComponent<testComponent>(currEnt))
			{
				if (CompFilter.PassFilter("test") && ECGui::CreateCollapsingHeader("test"))
				{
				}
			}

			AddComponentsController(currEnt);
			RemoveComponentsController(currEnt);
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

				ECGui::DrawTextWidget<std::string>("Entity Tag", lexical_cast_toStr<EntityType>(entCom.Tag));
				ECGui::DrawTextWidget<const char*>("Edit Name:", "");
				ECGui::InsertSameLine();
				ECGui::DrawInputTextHintWidget("InputEntityName", "Enter Entity Name", entNameInput, 256);

				if (ECGui::ButtonBool("Set Name"))
				{
					std::string oldName = entCom.Name;
					entCom.Name = entNameInput;
					CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<std::string>{ oldName, entCom.Name });
				}

				/*static char test[256];
				static std::string testtest;
				ECGui::DrawInputTextHintWidget("test", "testtest", test, 256);
				engine->editorManager->Item_.GenericPayloadTarget("TESTING", testtest, "SUCCESSFUL");
				strcpy(test, testtest.c_str());*/
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

	bool InspectorWindow:: ShowPointLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
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
	void InspectorWindow::AddComponentsController( Entity ID)
	{
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");
		}
		if (ImGui::BeginPopup("Add Component"))
		{
			AddComponents(ID);
			ImGui::EndPopup();
		}
	}
	void InspectorWindow::RemoveComponentsController(Entity ID)
	{
		if (ImGui::Button("Remove Component"))
		{
			ImGui::OpenPopup("Remove Component");
		}
		if (ImGui::BeginPopup("Remove Component"))
		{
			RemoveComponents(ID);
			ImGui::EndPopup();
		}
	}
	void InspectorWindow::AddComponents(Entity ID)
	{

		static ImGuiTextFilter AddComponentFilter;
		
		auto& entCom = engine->world.GetComponent<EntityComponent>(ID);
		
		AddComponentFilter.Draw();

		for (int i = 0; i < engine->world.GetAllComponentNames().size(); i++)
		{
			if (AddComponentFilter.PassFilter(engine->world.GetAllComponentNames().at(i).c_str()))
			{
				if(ImGui::Button(engine->world.GetAllComponentNames().at(i).c_str(), ImVec2(200, 0)))
				{
					switch (str2int(engine->world.GetAllComponentNames().at(i).c_str()))
					{
					case str2int("TransformComponent"):
						if (!engine->world.CheckComponent<TransformComponent>(ID))
						{
							AddComponentsSucess("TransformComponent", entCom, ID);
							engine->world.AddComponent(ID, TransformComponent{});
						}
						else
						{
							AddComponentsFailed("TransformComponent", entCom, ID);
						}
						break;
					case str2int("RenderComponent"):
						if (!engine->world.CheckComponent<RenderComponent>(ID))
						{
							AddComponentsSucess("RenderComponent", entCom, ID);
							engine->world.AddComponent(ID, RenderComponent{});
						}
						else
						{
							AddComponentsFailed("RenderComponent", entCom, ID);
						}
						break;
					case str2int("CameraComponent"):
						if (!engine->world.CheckComponent<CameraComponent>(ID))
						{
							AddComponentsSucess("CameraComponent", entCom, ID);
							engine->world.AddComponent(ID, CameraComponent{});
						}
						else
						{
							AddComponentsFailed("CameraComponent", entCom, ID);
						}
						break;
					case str2int("PointLightComponent"):
						if (!engine->world.CheckComponent<PointLightComponent>(ID))
						{
							AddComponentsSucess("PointLightComponent", entCom, ID);
							engine->world.AddComponent(ID, PointLightComponent{});
						}
						else
						{
							AddComponentsFailed("PointLightComponent", entCom, ID);
						}
						break;
					case str2int("DirectionalLightComponent"):
						if(!engine->world.CheckComponent<DirectionalLightComponent>(ID))
						{
							AddComponentsSucess("DirectionalLightComponent", entCom, ID);
							engine->world.AddComponent(ID, DirectionalLightComponent{});
						}
						else
						{
							AddComponentsFailed("DirectionalLightComponent", entCom, ID);
						}
						break;
					case str2int("AabbComponent"):
						if (!engine->world.CheckComponent<AabbComponent>(ID))
						{
							AddComponentsSucess("AabbComponent", entCom, ID);
							engine->world.AddComponent(ID, AabbComponent{});
						}
						else
						{
							AddComponentsFailed("AabbComponent", entCom, ID);
						}
						break;
					case str2int("SpotLightComponent"):
						if (!engine->world.CheckComponent<SpotLightComponent>(ID))
						{
							AddComponentsSucess("SpotLightComponent", entCom, ID);
							engine->world.AddComponent(ID, SpotLightComponent{});
						}
						else
						{
							AddComponentsFailed("SpotLightComponent", entCom, ID);
						}
						break;
					case str2int("MaterialComponent"):
						if (!engine->world.CheckComponent<MaterialComponent>(ID))
						{
							AddComponentsSucess("MaterialComponent", entCom, ID);
							engine->world.AddComponent(ID, MaterialComponent{});
						}
						else
						{
							AddComponentsFailed("MaterialComponent", entCom, ID);
						}
						break;
					case str2int("testComponent"):
						if (!engine->world.CheckComponent<testComponent>(ID))
						{
							AddComponentsSucess("testComponent", entCom, ID);
							engine->world.AddComponent(ID, testComponent{});
						}
						else
						{
							AddComponentsFailed("testComponent",entCom,ID);
						}
						break;
					}
				}
			}
		}
		
	}
	void InspectorWindow::AddComponentsSucess(const char* Components, EntityComponent& entCom, Entity ID)
	{
		std::string Comp(Components);
		Comp += " Added For " + entCom.Name + std::to_string(ID) + " Add Succeed";
		EDITOR_LOG_INFO(Comp.c_str());
	}
	void InspectorWindow::AddComponentsFailed(const char* Components, EntityComponent& entCom, Entity ID)
	{
		std::string Comp(Components);
		Comp += " Already Exists in " + entCom.Name + std::to_string(ID) + " Add Failed";
		EDITOR_LOG_WARN(Comp.c_str());
	}
	void InspectorWindow::RemoveComponents(Entity ID)
	{
		static ImGuiTextFilter AddComponentFilter;
		
		auto& entCom = engine->world.GetComponent<EntityComponent>(ID);
		
		AddComponentFilter.Draw();

		for (int i = 0; i < engine->world.GetAllComponentNames().size(); i++)
		{
			if (AddComponentFilter.PassFilter(engine->world.GetAllComponentNames().at(i).c_str()))
			{
				if(ImGui::Button(engine->world.GetAllComponentNames().at(i).c_str(), ImVec2(200, 0)))
				{
					switch (str2int(engine->world.GetAllComponentNames().at(i).c_str()))
					{
					case str2int("TransformComponent"):
						if (engine->world.CheckComponent<TransformComponent>(ID))
						{
							RemoveComponentsSucess("TransformComponent", entCom, ID);
							engine->world.DestroyComponent<TransformComponent>(ID);
						}
						else
						{
							RemoveComponentsFailed("TransformComponent", entCom, ID);
						}
						break;
					case str2int("RenderComponent"):
						if (engine->world.CheckComponent<RenderComponent>(ID))
						{
							RemoveComponentsSucess("RenderComponent", entCom, ID);
							engine->world.DestroyComponent<RenderComponent>(ID);
						}
						else
						{
							RemoveComponentsFailed("RenderComponent", entCom, ID);
						}
						break;
					case str2int("CameraComponent"):
						if (engine->world.CheckComponent<CameraComponent>(ID))
						{
							RemoveComponentsSucess("CameraComponent", entCom, ID);
							engine->world.DestroyComponent<CameraComponent>(ID);
						}
						else
						{
							RemoveComponentsFailed("CameraComponent", entCom, ID);
						}
						break;
					case str2int("PointLightComponent"):
						if (engine->world.CheckComponent<PointLightComponent>(ID))
						{
							RemoveComponentsSucess("PointLightComponent", entCom, ID);
							engine->world.DestroyComponent<PointLightComponent>(ID);
						}
						else
						{
							RemoveComponentsFailed("PointLightComponent", entCom, ID);
						}
						break;
					case str2int("DirectionalLightComponent"):
						if(engine->world.CheckComponent<DirectionalLightComponent>(ID))
						{
							RemoveComponentsSucess("DirectionalLightComponent", entCom, ID);
							engine->world.DestroyComponent<DirectionalLightComponent>(ID);
						}
						else
						{
							RemoveComponentsFailed("DirectionalLightComponent", entCom, ID);
						}
						break;
					case str2int("AabbComponent"):
						if (engine->world.CheckComponent<AabbComponent>(ID))
						{
							RemoveComponentsSucess("AabbComponent", entCom, ID);
							engine->world.DestroyComponent<AabbComponent>(ID);
						}
						else
						{
							RemoveComponentsFailed("AabbComponent", entCom, ID);
						}
						break;
					case str2int("SpotLightComponent"):
						if (engine->world.CheckComponent<SpotLightComponent>(ID))
						{
							RemoveComponentsSucess("SpotLightComponent", entCom, ID);
							engine->world.DestroyComponent<SpotLightComponent>(ID);
						}
						else
						{
							RemoveComponentsFailed("SpotLightComponent", entCom, ID);
						}
						break;
					case str2int("MaterialComponent"):
						if (engine->world.CheckComponent<MaterialComponent>(ID))
						{
							RemoveComponentsSucess("MaterialComponent", entCom, ID);
							engine->world.DestroyComponent<MaterialComponent>(ID);
						}
						else
						{
							RemoveComponentsFailed("MaterialComponent", entCom, ID);
						}
						break;
					case str2int("testComponent"):
						if (engine->world.CheckComponent<testComponent>(ID))
						{
							RemoveComponentsSucess("testComponent", entCom, ID);
							engine->world.DestroyComponent<testComponent>(ID);
						}
						else
						{
							RemoveComponentsFailed("testComponent",entCom,ID);
						}
						break;
					}
				}
			}
		}
		
	}
	void InspectorWindow::RemoveComponentsSucess(const char* Components, EntityComponent& entCom, Entity ID)
	{
		std::string Comp(Components);
		Comp += " Removed For " + entCom.Name + std::to_string(ID) + " Remove Succeed";
		EDITOR_LOG_INFO(Comp.c_str());
	}
	void InspectorWindow::RemoveComponentsFailed(const char* Components, EntityComponent& entCom, Entity ID)
	{
		std::string Comp(Components);
		Comp += " Does Not Exists in " + entCom.Name + std::to_string(ID) + " Remove Failed";
		EDITOR_LOG_WARN(Comp.c_str());
	}
}
