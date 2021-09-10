#include "pch.h"
#include "Inspector.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RigidBodyComponent.h"

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
			ShowRigidBodyProperty("RigidBody", currEnt, CompFilter);
			ShowEditorCameraProperty("Camera", currEnt, CompFilter);
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
	bool InspectorWindow::ShowRigidBodyProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
	{
		if (engine->world.CheckComponent<RigidBodyComponent>(ID))
		{
			if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
			{
				auto& _RigidB = engine->world.GetComponent<RigidBodyComponent>(ID);

				ECGui::DrawTextWidget<const char*>("Enable Gravity", "");
				ECGui::CheckBoxBool("Rigid Body Enable Gravity", &_RigidB.enableGravity);
				
				ECGui::DrawTextWidget<const char*>("Forces", "");
				ECGui::DrawSliderFloat3Widget("Rigid Body Forces", &_RigidB.forces, true, 0.0f, 1.0f);
				
				ECGui::DrawTextWidget<const char*>("Velocity", "");
				ECGui::DrawSliderFloat3Widget("Rigid Body Velocity", &_RigidB.velocity, true, 0.0f, 1.0f);

			}
		}

		return false;
	}
	bool InspectorWindow::ShowEditorCameraProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
	{
		if (engine->world.CheckComponent<CameraComponent>(ID))
		{
			if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
			{
				auto& _Camera = engine->world.GetComponent<CameraComponent>(ID);

				if(engine->gCamera.GetEditorCameraID() == ID)
				{
					ECGui::DrawTextWidget<const char*>("Near Plane", "");
					ECGui::DrawSliderFloatWidget("Near Plane", &_Camera.nearPlane, false, 0, 1000);
					ECGui::DrawTextWidget<const char*>("Far Plane", "");
					ECGui::DrawSliderFloatWidget("Far Plane", &_Camera.farPlane,false,0,1000);
				}
				ECGui::DrawTextWidget<const char*>("Camera Speed", "");
				ECGui::DrawSliderFloatWidget("Camera Speed", &_Camera.cameraSpeed);
				
				//ECGui::DrawTextWidget<const char*>("Enable Gravity", "");
				//ECGui::CheckBoxBool("Rigid Body Enable Gravity", &_RigidB.enableGravity);
				//
				//ECGui::DrawTextWidget<const char*>("Forces", "");
				//ECGui::DrawSliderFloat3Widget("Rigid Body Forces", &_RigidB.forces, true, 0.0f, 1.0f);
				//
				//ECGui::DrawTextWidget<const char*>("Velocity", "");
				//ECGui::DrawSliderFloat3Widget("Rigid Body Velocity", &_RigidB.velocity, true, 0.0f, 1.0f);

			}
		}

		return false;
	}
	void InspectorWindow::AddComponentsController( Entity ID)
	{
		ImVec2 buttonSize = {180,20};
		if (ImGui::Button(("Add Component"), buttonSize))
		{
			ImGui::OpenPopup("Add Component");
		}
		if (ImGui::BeginPopup("Add Component"))
		{
			ImGui::SetScrollY(5);
			ChildSettings settings{ "Add Components", ImVec2{ 250,200 } };
			ECGui::DrawChildWindow<void(Entity)>(settings, std::bind(&InspectorWindow::ShowAddComponentList,
				this, std::placeholders::_1), ID);
			ImGui::EndPopup();
		}
	}
	void InspectorWindow::RemoveComponentsController(Entity ID)
	{
		ImVec2 buttonSize = { 180,20 };

		if (ImGui::Button(("Remove Component"), buttonSize))
		{
			ImGui::OpenPopup("Remove Component");
		}
		if (ImGui::BeginPopup("Remove Component"))
		{
			ImGui::SetScrollY(5);
			ChildSettings settings{ "Remove Components", ImVec2{ 250,200 } };
			ECGui::DrawChildWindow<void(Entity)>(settings, std::bind(&InspectorWindow::ShowRemoveComponentList,
				this, std::placeholders::_1), ID);
			ImGui::EndPopup();
		}
	}
	void InspectorWindow::ShowAddComponentList(Entity ID)
	{
		static ImGuiTextFilter AddComponentFilter;

		auto& entCom = engine->world.GetComponent<EntityComponent>(ID);
		
		AddComponentFilter.Draw("Filter",160);

		for (int i = 0; i < engine->world.GetAllComponentNames().size(); i++)
		{
			if (AddComponentFilter.PassFilter(engine->world.GetAllComponentNames().at(i).c_str()))
			{
				if(ImGui::Button(engine->world.GetAllComponentNames().at(i).c_str(), ImVec2(200, 0)))
				{
					switch (str2int(engine->world.GetAllComponentNames().at(i).c_str()))
					{
					case str2int("TransformComponent"):
						ComponentRegistry<TransformComponent>("TransformComponent", ID, entCom.Name,
							EditComponent::EC_ADDCOMPONENT);
						break;
					case str2int("RenderComponent"):
						ComponentRegistry<RenderComponent>("RenderComponent", ID, entCom.Name,
							EditComponent::EC_ADDCOMPONENT);
						break;
					case str2int("CameraComponent"):
						ComponentRegistry<CameraComponent>("CameraComponent", ID, entCom.Name,
							EditComponent::EC_ADDCOMPONENT);
						break;
					case str2int("PointLightComponent"):
						ComponentRegistry<PointLightComponent>("PointLightComponent", ID, entCom.Name,
							EditComponent::EC_ADDCOMPONENT);
						break;
					case str2int("DirectionalLightComponent"):
						ComponentRegistry<DirectionalLightComponent>("DirectionalLightComponent", ID, entCom.Name,
							EditComponent::EC_ADDCOMPONENT);
						break;
					case str2int("AABBComponent"):
						ComponentRegistry<AABBComponent>("AABBComponent", ID, entCom.Name,
							EditComponent::EC_ADDCOMPONENT);
						break;
					case str2int("SpotLightComponent"):
						ComponentRegistry<SpotLightComponent>("SpotLightComponent", ID, entCom.Name,
							EditComponent::EC_ADDCOMPONENT);
						break;
					case str2int("MaterialComponent"):
						ComponentRegistry<MaterialComponent>("MaterialComponent", ID, entCom.Name,
							EditComponent::EC_ADDCOMPONENT);
						break;
					case str2int("RigidBodyComponent"):
						ComponentRegistry<RigidBodyComponent>("RigidBodyComponent", ID, entCom.Name,
							EditComponent::EC_ADDCOMPONENT);
						break;
					case str2int("testComponent"):
						ComponentRegistry<testComponent>("testComponent", ID, entCom.Name,
							EditComponent::EC_ADDCOMPONENT);
						break;
					}
				}
			}
		}
		
	}
	void InspectorWindow::AddComponentsSuccess(const char* Components, const std::string& name, Entity ID)
	{
		std::string Comp = my_strcat(std::string{ Components }, " Added For ", name, ID, " Add Succeed");
		/*std::string Comp(Components);
		Comp += " Added For " + name + std::to_string(ID) + " Add Succeed";*/
		EDITOR_LOG_INFO(Comp.c_str());
	}

	void InspectorWindow::AddComponentsFailed(const char* Components, const std::string& name, Entity ID)
	{
		std::string Comp = my_strcat(std::string{ Components }, " Already Exists in ", name, ID, " Add Failed");
		/*std::string Comp(Components);
		Comp += " Already Exists in " + name + std::to_string(ID) + " Add Failed";*/
		EDITOR_LOG_WARN(Comp.c_str());
	}

	void InspectorWindow::ShowRemoveComponentList(Entity ID)
	{
		static ImGuiTextFilter RemoveComponentFilter;
		
		auto& entCom = engine->world.GetComponent<EntityComponent>(ID);
		
		RemoveComponentFilter.Draw("Filter", 160);

		for (int i = 0; i < engine->world.GetAllComponentNames().size(); i++)
		{
			if (RemoveComponentFilter.PassFilter(engine->world.GetAllComponentNames().at(i).c_str()))
			{
				if(ImGui::Button(engine->world.GetAllComponentNames().at(i).c_str(), ImVec2(200, 0)))
				{
					switch (str2int(engine->world.GetAllComponentNames().at(i).c_str()))
					{
					case str2int("TransformComponent"):
						ComponentRegistry<TransformComponent>("TransformComponent", ID, entCom.Name,
							EditComponent::EC_REMOVECOMPONENT);
						break;
					case str2int("RenderComponent"):
						ComponentRegistry<RenderComponent>("RenderComponent", ID, entCom.Name,
							EditComponent::EC_REMOVECOMPONENT);
						break;
					case str2int("CameraComponent"):
						ComponentRegistry<CameraComponent>("CameraComponent", ID, entCom.Name,
							EditComponent::EC_REMOVECOMPONENT);
						break;
					case str2int("PointLightComponent"):
						ComponentRegistry<PointLightComponent>("PointLightComponent", ID, entCom.Name,
							EditComponent::EC_REMOVECOMPONENT);
						break;
					case str2int("DirectionalLightComponent"):
						ComponentRegistry<DirectionalLightComponent>("DirectionalLightComponent", ID, entCom.Name,
							EditComponent::EC_REMOVECOMPONENT);
						break;
					case str2int("AABBComponent"):
						ComponentRegistry<AABBComponent>("AABBComponent", ID, entCom.Name,
							EditComponent::EC_REMOVECOMPONENT);
						break;
					case str2int("SpotLightComponent"):
						ComponentRegistry<SpotLightComponent>("SpotLightComponent", ID, entCom.Name,
							EditComponent::EC_REMOVECOMPONENT);
						break;
					case str2int("MaterialComponent"):
						ComponentRegistry<MaterialComponent>("MaterialComponent", ID, entCom.Name,
							EditComponent::EC_REMOVECOMPONENT);
						break;
					case str2int("RigidBodyComponent"):
						ComponentRegistry<RigidBodyComponent>("RigidBodyComponent", ID, entCom.Name,
							EditComponent::EC_REMOVECOMPONENT);
						break;
					case str2int("testComponent"):
						ComponentRegistry<testComponent>("testComponent", ID, entCom.Name,
							EditComponent::EC_REMOVECOMPONENT);
						break;
					}
				}
			}
		}
		
	}
	void InspectorWindow::RemoveComponentsSuccess(const char* Components, const std::string& name, Entity ID)
	{
		std::string Comp = my_strcat(std::string{ Components }, " Removed For ", name, ID, " Remove Succeed");
		/*std::string Comp(Components);
		Comp += " Removed For " + entCom.Name + std::to_string(ID) + " Remove Succeed";*/
		EDITOR_LOG_INFO(Comp.c_str());
	}
	void InspectorWindow::RemoveComponentsFailed(const char* Components, const std::string& name, Entity ID)
	{
		std::string Comp = my_strcat(std::string{ Components }, " Does Not Exists in ", name, ID, " Remove Failed");
		/*std::string Comp(Components);
		Comp += " Does Not Exists in " + entCom.Name + std::to_string(ID) + " Remove Failed";*/
		EDITOR_LOG_WARN(Comp.c_str());
	}
}
