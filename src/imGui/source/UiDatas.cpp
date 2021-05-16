/**********************************************************************************
* \file            UiDatas.cpp
*
* \brief        Definition of the UiData class function(s).
*
* \author        Tian Yu
*
* \email		t.yu\@digipen.edu
*
* \date			9 oct 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "UiDatas.h"

#include <imgui_internal.h>

#include "AllComponents.h"
#include "World.h"
#include "AllComponentFunctions.h"
#include "Editor.h"
#include "EditorPause.h"
#include "GameUi.h"
#include "ParticleSystem.h"
#include "StatesComponent.h"
#include "UiAddName.h"
#include "UiCollisionDatas.h"

extern bool ClimbSliding;

void UiData::Gui_Datas(World& world, bool& scroll)
{
	World* tempW = &(world);
	unsigned int ID;
	static bool NameEdit = false;
	static bool createParticle = false;
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	//static std::array<char,100>  nameCollector = { "" };
	//static std::map <int, std::array<char, 100>> nameMap;
	auto& entityManager = tempW->GetEntityManager();
	auto& manager = tempW->GetComponentManager();
	auto lookup = manager.GetComponentLookup<IsActive>();
	static int offset_x = 0;
	ImGui::SetNextItemWidth(100);
	ImGui::DragInt("Pos of box below", &offset_x, 1.0f, -1000, 1000);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (float)offset_x);
	ImGui::BeginChild("Obj Data", ImVec2(0,  500), true);
	ImGuiTreeNodeFlags node_flags = base_flags;
	static ImGuiTextFilter filter;
	static int tempButtoniD = 0;
	static bool readytoset = false;
	static bool readytoremove = false;
	filter.Draw("Filter Names");
	for (auto& pair : lookup)
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
		ID = pair.first;
		std::string name = UiAddName::GetName(ID);
		ImGui::PushID(ID);
		if (!filter.PassFilter(name.c_str()))
		{
			continue;
		}
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
		if (ImGui::TreeNode(name.c_str()))
		{
			ImGui::Checkbox("Edit Name", &NameEdit);
			if (NameEdit)
			{
				UiAddName::AddName(ID,NameEdit);
			}
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui_LayOut::Gui_InsertSeparator(1);
			auto& _RigidBody = manager.GetComponent<RigidBody>(pair.first).get();
			auto& sprite = manager.GetComponent<Sprite>(pair.first).get();
			auto checkSprite = manager.GetComponent<Sprite>(pair.first).has();
			auto BoundingBoxCheck = manager.GetComponent<BoundingBox>(pair.first).has();
			auto Player_Check = manager.GetComponent<Player>(pair.first).has();
			auto Enemy_Check = manager.GetComponent<Enemy>(pair.first).has();
			auto& _transform = manager.GetComponent<Transform>(pair.first).get();
			auto& _Combat = manager.GetComponent<Combat>(pair.first).get();
			auto checkEmitter = manager.GetComponent<Emitter>(pair.first).has();
			auto& myEmitter = manager.GetComponent<Emitter>(pair.first).get();
			auto checkProj = manager.GetComponent<Projectile>(pair.first).has();
			auto& myProj = manager.GetComponent<Projectile>(pair.first).get();
			auto checkAudio = manager.GetComponent<AudioComponent>(pair.first).has();
			auto& myAudio = manager.GetComponent<AudioComponent>(pair.first).get();
			auto checkButton = manager.GetComponent<Buttons>(pair.first).has();
			auto checkUi = manager.GetComponent<Ui>(pair.first).has();

			// Platform Status
			auto& _PlatformInformation = manager.GetComponent<Platforms>(pair.first).get();
			auto _CheckPlatform = manager.GetComponent<Platforms>(pair.first).has();

			//Player 
			auto CheckPlayer = manager.GetComponent<Player>(pair.first).has();

			if (CheckPlayer)
			{
				if (ImGui::CollapsingHeader("PLAYER INFORMATION", ImGuiTreeNodeFlags_None))
				{
					auto& PlayeBody = manager.GetComponent<RigidBody>(pair.first).get();
					auto& _Player = manager.GetComponent<Player>(pair.first).get();

					ImGui::Dummy(ImVec2(0.0f, 20.0f));
					ImGui::DragFloat2("Velocity D", (float*)&PlayeBody.velocitydirection);
					ImGui::DragFloat2("Acceleration", (float*)&PlayeBody.acceleration);
					ImGui::DragFloat2("Force", (float*)&PlayeBody.force);
					ImGui::DragFloat("Mass ", (float*)&PlayeBody.mass);
					ImGui::Dummy(ImVec2(0.0f, 20.0f));
					ImGui::Checkbox("Is Sliding?", &ClimbSliding);
					ImGui::Dummy(ImVec2(0.0f, 20.0f));

					// First Action
					ImGui::BeginChild("Components", ImVec2(0, 40), true);

					if (PlayeBody._Action == Action::CLIMBING)
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Action : ClIMBING");
					else if (PlayeBody._Action == Action::SLIDINGDOWN)
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Action : SLIDINGDOWN");
					else if (PlayeBody._Action == Action::WalkingLeft)
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Action : WalkingLeft");
					else if (PlayeBody._Action == Action::WalkingRight)
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Action : WalkingRight");
					else if (PlayeBody._Action == Action::Jump)
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Action : Jumping");
					else if (PlayeBody._Action == Action::Idle)
					{
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Action : idle");
					}

					if (PlayeBody._Prev == Action::CLIMBING)
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Prev Action : ClIMBING");
					else if (PlayeBody._Prev == Action::SLIDINGDOWN)
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Prev Action : SLIDINGDOWN");
					else if (PlayeBody._Prev == Action::WalkingLeft)
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Prev Action : WalkingLeft");
					else if (PlayeBody._Prev == Action::WalkingRight)
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Prev Action : WalkingRight");
					else if (PlayeBody._Prev == Action::Jump)
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Prev Action : Jumping");
					else if (PlayeBody._Prev == Action::Idle)
					{
						ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%s", "Prev Action : idle");
					}

					ImGui::Separator();
					ImGui::EndChild();

					ImGui::Dummy(ImVec2(0.0f, 20.0f));
				}
			}

			if (_CheckPlatform)
			{
				if (ImGui::CollapsingHeader("Button", ImGuiTreeNodeFlags_None))
				{
					if (_PlatformInformation._Type == Platforms::PlatformType::BOUNCEUP)
					{
						ImGui::Checkbox("SWITCH ON", &_PlatformInformation.SwitchOn);
						ImGui::DragFloat("FORCE", (float*)&_PlatformInformation.ForceY, 0.0f, 0, 5000.0f);
					}
				}
			}

			if (ImGui::CollapsingHeader("Object Info", ImGuiTreeNodeFlags_None))
			{
				ImGui::DragFloat2("Scale", (float*)&_transform.scale, 1.0, 0.0f, 5000.f);
				ImGui::DragFloat2("Translation", (float*)&_transform.position);
				ImGui::DragFloat("Rotation", (float*)&_transform.rotation, 1.0f, 0.0f, 360.0f);
				ImGui::Dummy(ImVec2(0.0f, 20.0f));
			}
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui_LayOut::Gui_InsertSeparator(1);
			auto& myUi = manager.GetComponent<Ui>(pair.first).get();
			if (checkUi)
			{
				if (ImGui::CollapsingHeader("Ui", ImGuiTreeNodeFlags_None))
				{
					ImGui::DragFloat2("UiPosActive", (float*)&myUi.UiPos);
					_transform.position.x = myUi.UiPos.x;
					_transform.position.y = myUi.UiPos.y;
					ImGui::DragFloat2("isMov", (float*)&myUi.isMovPos);
					ImGui::Combo("Set MainMenu", &myUi.MainMenuselector,
						"None\0Quit\0Level1Play\0yes\0no\0next\0back\0titlescreen\0retry\0");
					ImGui::Combo("Set Level", &myUi.Levelselector,
						"None\0HowtoPlay\0Quit\0Resume\0HowtoPlayMenu\0HowtoPlayPanel\0HowtoPlayBack\0HowtoPlayNxt\0HowtoPlayClose\0hp\0progressbar\0pausebg\0suretoquitbg\0yes\0no\0");
					GameUi::MainMenuSelector(myUi.MainMenuselector, myUi);
					GameUi::LevelSelector(myUi.Levelselector, myUi);
					ImGui::Checkbox("Is Ui Active?", &myUi.isActive);
				}
			}
			if (checkButton)
			{
				if (ImGui::CollapsingHeader("Button", ImGuiTreeNodeFlags_None))
				{
					if (ImGui::Button("Set Button and links"))
					{
						ImGui::OpenPopup("Set Button and links");
					}
					if (ImGui::BeginPopupModal("Set Button and links"))
					{
						ImGui::Text("After pressing okay click on an object and press L to link\n", name.c_str());
						ImGui::Separator();

						static bool dont_ask_me_next_time = false;
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
						ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
						ImGui::PopStyleVar();

						if (ImGui::Button("OK", ImVec2(200, 0)))
						{
							Interactables::SetButtonID(ID, &world);
							readytoset = true;
							tempButtoniD = ID;
							ImGui::CloseCurrentPopup();

						}
						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();
						if (ImGui::Button("Cancel", ImVec2(200, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();

					}
					if (ImGui::Button("Remove Links"))
					{
						ImGui::OpenPopup("Remove Links");
					}
					if (ImGui::BeginPopupModal("Remove Links"))
					{
						ImGui::Text("After pressing okay click on an object and press L to delink\n", name.c_str());
						ImGui::Separator();

						static bool dont_ask_me_next_time = false;
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
						ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
						ImGui::PopStyleVar();

						if (ImGui::Button("OK", ImVec2(200, 0)))
						{
							Interactables::SetButtonID(ID, &world);
							readytoremove = true;
							tempButtoniD = ID;
							ImGui::CloseCurrentPopup();
						}
						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();
						if (ImGui::Button("Cancel", ImVec2(200, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();

					}
				}
			}
			if (readytoremove)
			{
				int L = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_L);
				if (GLFW_PRESS == L)
				{
					if (Interactables::isButton(tempButtoniD, &world))
					{
						std::cout << "TempID: " << tempButtoniD << std::endl;
						Interactables::RemoveButtonInteractable(tempButtoniD, ID, &world);
						int counter = 0;
						for (auto& i : Graphics::interactables)
						{
							++counter;
							std::cout << "First: " << i.first << std::endl;
							std::cout << "Second: " << i.second << std::endl;
							std::cout << "counter: " << counter << std::endl;
						}
						readytoremove = false;
					}
				}
			}
			if (readytoset)
			{
				int L = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_L);
				if (GLFW_PRESS == L)
				{
					if (Interactables::isButton(tempButtoniD, &world))
					{
						std::cout << "TempID: " << tempButtoniD << std::endl;
						Interactables::AddButtonInteractable(tempButtoniD, ID, &world);

						int counter = 0;
						for (auto& i : Graphics::interactables)
						{
							++counter;
							std::cout << "First: " << i.first << std::endl;
							std::cout << "Second: " << i.second << std::endl;
							std::cout << "counter: " << counter << std::endl;
						}
						readytoset = false;
					}
				}
			}
			if (Enemy_Check)
			{
				if (ImGui::CollapsingHeader("Enemy Info", ImGuiTreeNodeFlags_None))
				{
					ImGui::DragFloat("Enemy Max Health", &_Combat.p_health.Max_Health, 1.0f, 0, 100);
					ImGui::DragFloat("Enemy Current Health", &_Combat.p_health.Curr_Health, 1.0f, 0, 100);
					ImGui::DragInt("Enemy Damage", (int*)&_Combat.damage, 1.0f, 0, 100);
				}
			}
			if (checkEmitter)
			{
				if (ImGui::CollapsingHeader("Particle Emitter", ImGuiTreeNodeFlags_None))
				{
					ImGui_LayOut::Gui_InsertSeparator(1);
					ImGui::DragInt("Particle Num", &myEmitter.maxParticle, 1, 1, 3);
					ImGui::DragFloat2("Vel", (float*)&myEmitter.my_particle.vel, 0.5, 1, 1000);
					ImGui::DragFloat("LifeTime", (float*)&myEmitter.my_particle.lifeTime, 0.1f, 0.1f, 3.0f);
					ImGui::ColorEdit4("colorStart", (float*)&myEmitter.my_particle.colorStart);
					ImGui::ColorEdit4("colorEnd", (float*)&myEmitter.my_particle.colorEnd);
					ImGui::DragFloat("sizeStart", (float*)&myEmitter.my_particle.sizeStart, 0.5, 1, 1000);
					ImGui::DragFloat("sizeEnd", (float*)&myEmitter.my_particle.sizeEnd, 0.5, 0, 1000);
					ImGui::DragFloat("RandomSizeFrom", (float*)&myEmitter.my_particle.randomVariation1, 0.001f, 0.001f, 1);
					ImGui::DragFloat("RandomSizeTo", (float*)&myEmitter.my_particle.randomVariation2, 0.001f, 0.001f, 1);
					ImGui::SameLine();
					ImGui_LayOut::HelpMarker("RandomSizeFrom and RandomSizeTo cannot be set to 0\n");
					ImGui::DragFloat("DeltaTimeMultipler", &myEmitter.my_particle.deltaTimeMultiplier, 0.5, 1, 1000);
					ImGui::DragInt("Particle Layer", (int*)&myEmitter.my_particle.particleLayer, 1, 1, 40);
					static int particalSelector = -1;
					if (ImGui::Combo("Particle type", &particalSelector, "Dust\0Custom\0others\0"))
					{
						ParticleSystem::particleSelector(particalSelector, &myEmitter);
						ImGui_LayOut::Gui_InsertSeparator(2);
					}
					ImGui::Checkbox("Preview Particle", &createParticle);
					ImGui::SameLine();
					ImGui_LayOut::HelpMarker("Create particle for preview\n");
					if (createParticle)
					{
						ParticleSystem::CreateParticle(&world, myEmitter, _transform);
					}
					ImGui::Checkbox("Set Particle", &myEmitter.my_particle.setParticle);
					ImGui::SameLine();
					ImGui_LayOut::Gui_InsertSeparator(1);
				}
			}
			if (Player_Check)
			{
				if (ImGui::CollapsingHeader("Player Info", ImGuiTreeNodeFlags_None))
				{
					_Combat.p_health.Max_Health = 3;
					_Combat.Type_Of_Entity = EntityFlag::Player;
					//_Combat.p_health.Curr_Health = 3;
					ImGui::DragFloat("Player Current Health", &_Combat.p_health.Curr_Health, 1.0f, 0, 100);
					//_Combat.damage = 2;
					ImGui::DragInt("Player Damage", (int*)&_Combat.damage, 1.0f, 0, 100);

				}
			}
			if (checkProj)
			{
				if (ImGui::CollapsingHeader("Projectile Info", ImGuiTreeNodeFlags_None))
				{
					ImGui_LayOut::Gui_InsertSeparator(1);
					static int projSelector; 
					projSelector = static_cast<int>(myProj.type);
					if (ImGui::Combo("Proj type", &projSelector, "No type\0Boomerang\0Pebble\0"))
					{
						myProj.type = static_cast<ProjectileType>(projSelector);
						ImGui_LayOut::Gui_InsertSeparator(2);
					}
					static int projState;
					projState = static_cast<int>(myProj.state);
					if (ImGui::Combo("Proj state", &projState, "Idle\0Holded\0Flying\0Hit\0ComingBack\0Drop"))
					{
						ImGui_LayOut::Gui_InsertSeparator(2);
					}
					ImGui::DragFloat("Proj speedX", &myProj.speed, 1.0f);
					ImGui::DragFloat("Proj speedY", &myProj.speedY, 1.0f);
					ImGui_LayOut::Gui_InsertSeparator(1);
				}
			}
			if (BoundingBoxCheck)
			{
				auto& editBoundingBox = manager.GetComponent<BoundingBox>(pair.first).get();
				ImGui::Checkbox("Edit Bounding Box", &editBoundingBox.IsManual);
				//editBoundingBox.IsManual = enableEdit;
				if (editBoundingBox.IsManual)
				{
					ImGui::DragFloat("Max X offset: ", &editBoundingBox.Offset_MaxX, 1.0f, 0.0f, 0.0f);
					ImGui::DragFloat("Max Y offset: ", &editBoundingBox.Offset_MaxY, 1.0f, 0.0f, 0.0f);
					ImGui::DragFloat("Min X offset: ", &editBoundingBox.Offset_MinX, 1.0f, 0.0f, 0.0f);
					ImGui::DragFloat("Min Y offset: ", &editBoundingBox.Offset_MinY, 1.0f, 0.0f, 0.0f);
				}
			}
			if (checkSprite)
			{
				if (ImGui::CollapsingHeader("Sprite Info", ImGuiTreeNodeFlags_None))
				{
					UiObjTypes::ObjectTypes(_RigidBody, name);
					ImGui::ColorEdit3("Color", (float*)&sprite.color);
					ImGui::SliderInt("Layer", (int*)(&sprite.newLayer), 1, 40);
					SpriteRenderingSystem::SetLayerNum(pair.first, sprite.newLayer, &world);
					ImGui::Dummy(ImVec2(0.0f, 20.0f));
					ImGui_LayOut::Gui_InsertSeparator(1);
					if (sprite.isQuad)
					{
						int j = 0;
						int counter = 0;

						if (ImGui::CollapsingHeader("Select Textures "))
						{
							if (ImGui::Button("TextureList"))
							{
								ImGui::OpenPopup("TextureList");
							}
							ImGui::SameLine();
							ImGui::TextUnformatted(sprite.hasTexture == true ? sprite.textureRef->first.c_str() : "<None>");
							if (ImGui::BeginPopup("TextureList"))
							{
								for (auto& i : Graphics::textures)
								{
									if (ImGui::Selectable(i.first.c_str()))
									{
										sprite.textureRef = Graphics::textures.find(i.first.c_str());
										sprite.hasTexture = true;

										if (Interactables::isButton(pair.first, &world))
										{
											Interactables::UpdateButtonType(pair.first, &world);
										}
									}
								}
								ImGui::EndPopup();
							}

							if (sprite.hasTexture)
							{
								auto checkAnimation = manager.GetComponent<Animation>(pair.first).has();
								if (checkAnimation)
								{
									auto& _animation = manager.GetComponent<Animation>(pair.first).get();									
									ImGui::DragFloat("Animation Speed", &_animation.frameTime, 0.01f, 0.0f, 1.f);
								}
								
								for (int i = 0; i < (sprite.textureRef->second.GetCols() * sprite.textureRef->second.GetRows()); ++i)
								{
									float offsetX = static_cast<float>((1.0 / sprite.textureRef->second.GetCols()) * i);
									float offsetY = static_cast<float>((1.0 / sprite.textureRef->second.GetRows()) * j);
									//size_t textureHandle = static_cast<size_t>(pair.second->textureRef->second.GetHandle());

									ImGui::PushID(i);

									if (ImGui::ImageButton((void*)sprite.textureRef->second.GetHandle(),
										ImVec2(32, 32),
										ImVec2(offsetX, offsetY),
										ImVec2(1 / sprite.textureRef->second.GetCols() + offsetX, 1 / sprite.textureRef->second.GetRows() + offsetY)))
									{
										sprite.textureIdx = glm::vec2(i, j);
									}
									ImGui::PopID();

									if (counter == (sprite.textureRef->second.GetCols() - 1))
									{
										ImGui::NewLine();
										counter = 0;
										++j;
									}
									else
									{
										ImGui::SameLine();
										++counter;
									}
								}
							}
							ImGui_LayOut::Gui_InsertSeparator(4);
						}

					}
					ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 102, 0, 100));
					ImGui::BeginChild("Body Type: ", ImVec2(0, 20), true);
					switch (_RigidBody._BodyType)
					{
					case BodyType::Null:
						ImGui::TextColored(ImVec4(1.0f, 1.6f, 1.1f, 1.0f), "Body Type: Null");
						break;
					case BodyType::Dynamic:
						ImGui::TextColored(ImVec4(1.0f, 1.6f, 1.1f, 1.0f), "Body Type: Dynamic");
						break;
					case BodyType::Kinematic:
						ImGui::TextColored(ImVec4(1.0f, 1.6f, 1.1f, 1.0f), "Body Type: Kinematic");
						break;
					case BodyType::Static:
						ImGui::TextColored(ImVec4(1.0f, 1.6f, 1.1f, 1.0f), "Body Type: Static");
						break;
					case BodyType::MaxCount:
						break;
					}
					ImGui::PopStyleColor();
					ImGui::EndChild();
				}
			}
			manager.PrintComponentsToGUI(ID);
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui_LayOut::Gui_InsertSeparator(1);
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::Button("Delete Entity"))
			{
				ImGui::OpenPopup("Delete Entity");
			}
			if (ImGui::BeginPopupModal("Delete Entity"))
			{
				ImGui::Text("Are you sure to Delete Entity %s ?\n\n", name.c_str());
				ImGui::Separator();

				static bool dont_ask_me_next_time = false;
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
				ImGui::PopStyleVar();

				if (ImGui::Button("OK", ImVec2(200, 0)))
				{
					LogicSystem::Others::UnloadEnemy_HpBars(ID, &world);
					entityManager.UnloadEntity(ID);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(200, 0))) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();

			}
			if (ImGui::Button("Add Component"))
				{
					ImGui::OpenPopup("Add Component");
				}
			if (ImGui::BeginPopup("Add Component"))
				{
					if (ImGui::Button("Add Transform"))
					{
						ImGui::OpenPopup("Add Transform?");
					}
					if (!manager.HasComponent<Transform>(ID))
					{
						LevelEditor::AddComponents("Transform", name, &manager, 0, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Transform", name);
					}
					if (ImGui::Button("Add BoundingBox"))
					{
						ImGui::OpenPopup("Add BoundingBox?");
					}
					if (!manager.HasComponent<BoundingBox>(ID))
					{
						LevelEditor::AddComponents("BoundingBox", name, &manager, 1, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("BoundingBox", name);
					}
					if (ImGui::Button("Add RigidBody"))
					{
						ImGui::OpenPopup("Add RigidBody?");
					}
					if (!manager.HasComponent<RigidBody>(ID))
					{
						LevelEditor::AddComponents("RigidBody", name, &manager, 2, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("RigidBody", name);
					}
					if (ImGui::Button("Add Sprite"))
					{
						ImGui::OpenPopup("Add Sprite?");
					}
					if (!manager.HasComponent<Sprite>(ID))
					{
						LevelEditor::AddComponents("Sprite", name, &manager, 3, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Sprite", name);
					}
					if (ImGui::Button("Add Animation"))
					{
						ImGui::OpenPopup("Add Animation?");
					}
					if (!manager.HasComponent<Animation>(ID))
					{
						LevelEditor::AddComponents("Animation", name, &manager, 4, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Animation", name);
					}
					if (ImGui::Button("Add Player"))
					{
						ImGui::OpenPopup("Add Player?");
					}
					if (!manager.HasComponent<Player>(ID))
					{
						LevelEditor::AddComponents("Player", name, &manager, 5, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Player", name);
					}
					if (ImGui::Button("Add Camera"))
					{
						ImGui::OpenPopup("Add Camera?");
					}
					if (!manager.HasComponent<Camera>(ID))
					{
						LevelEditor::AddComponents("Camera", name, &manager, 6, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Camera", name);
					}
					if (ImGui::Button("Add isActive"))
					{
						ImGui::OpenPopup("Add isActive?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (!manager.HasComponent<IsActive>(ID))
					{
						LevelEditor::AddComponents("isActive", name, &manager, 7, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("isActive", name);
					}
					if (ImGui::Button("Add Ui"))
					{
						ImGui::OpenPopup("Add Ui?");
					}
					if (!manager.HasComponent<Ui>(ID))
					{
						LevelEditor::AddComponents("Ui", name, &manager, 8, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Ui", name);
					}
					if (ImGui::Button("Add Combat"))
					{
						ImGui::OpenPopup("Add Combat?");
					}
					if (!manager.HasComponent<Combat>(ID))
					{
						LevelEditor::AddComponents("Combat", name, &manager, 9, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Combat", name);
					}
					if (ImGui::Button("Add Enemy"))
					{
						ImGui::OpenPopup("Add Enemy?");
					}
					if (!manager.HasComponent<Enemy>(ID))
					{
						LevelEditor::AddComponents("Enemy", name, &manager, 10, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Enemy", name);
					}
					if (ImGui::Button("Add Logic"))
					{
						ImGui::OpenPopup("Add Logic?");
					}
					if (!manager.HasComponent<Logic>(ID))
					{
						LevelEditor::AddComponents("Logic", name, &manager, 11, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Logic", name);
					}
					if (ImGui::Button("Add Emitter"))
					{
						ImGui::OpenPopup("Add Emitter?");
					}
					if (!manager.HasComponent<Emitter>(ID))
					{
						LevelEditor::AddComponents("Emitter", name, &manager, 12, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Emitter", name);
					}
					if (ImGui::Button("Add Projectile"))
					{
						ImGui::OpenPopup("Add Projectile?");
					}
					if (!manager.HasComponent<Projectile>(ID))
					{
						LevelEditor::AddComponents("Projectile", name, &manager, 13, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Projectile", name);
					}
					if (ImGui::Button("Add Button"))
					{
						ImGui::OpenPopup("Add Button?");
					}
					if (!manager.HasComponent<Buttons>(ID))
					{
						//Note: 14 is used for the AudioComponent
						LevelEditor::AddComponents("Button", name, &manager, 15, ID, world);
					}
					else
					{
						LevelEditor::FailedComponents("Button", name);
					}
					ImGui::EndPopup();
				}
			if (ImGui::Button("Remove Component"))
			{
				ImGui::OpenPopup("Remove Component");
			}
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui_LayOut::Gui_InsertSeparator(1);
			if (ImGui::BeginPopup("Remove Component"))
			{
					if (ImGui::Button("Remove Transform"))
					{
						ImGui::OpenPopup("Remove Transform?");
					}
					if (manager.HasComponent<Transform>(ID))
					{
						LevelEditor::RemoveComponents("Transform", name, &manager, 0, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Transform", name);
					}
					if (ImGui::Button("Remove BoundingBox"))
					{
						ImGui::OpenPopup("Remove BoundingBox?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<BoundingBox>(ID))
					{
						LevelEditor::RemoveComponents("BoundingBox", name, &manager, 1, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("BoundingBox", name);
					}
					if (ImGui::Button("Remove RigidBody"))
					{
						ImGui::OpenPopup("Remove RigidBody?");
					}
					if (manager.HasComponent<RigidBody>(ID))
					{
						LevelEditor::RemoveComponents("RigidBody", name, &manager, 2, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("RigidBody", name);
					}
					if (ImGui::Button("Remove Sprite"))
					{
						ImGui::OpenPopup("Remove Sprite?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<Sprite>(ID))
					{
						LevelEditor::RemoveComponents("Sprite", name, &manager, 3, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Sprite", name);
					}
					if (ImGui::Button("Remove Animation"))
					{
						ImGui::OpenPopup("Remove Animation?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<Animation>(ID))
					{
						LevelEditor::RemoveComponents("Animation", name, &manager, 4, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Animation", name);
					}
					if (ImGui::Button("Remove Player"))
					{
						ImGui::OpenPopup("Remove Player?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<Player>(ID))
					{
						LevelEditor::RemoveComponents("Player", name, &manager, 5, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Player", name);
					}
					if (ImGui::Button("Remove Camera"))
					{
						ImGui::OpenPopup("Remove Camera?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<Camera>(ID))
					{
						LevelEditor::RemoveComponents("Camera", name, &manager, 6, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Camera", name);
					}
					if (ImGui::Button("Remove isActive"))
					{
						ImGui::OpenPopup("Remove isActive?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<IsActive>(ID))
					{
						LevelEditor::RemoveComponents("isActive", name, &manager, 7, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("isActive", name);
					}
					if (ImGui::Button("Remove Ui"))
					{
						ImGui::OpenPopup("Remove Ui?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<Ui>(ID))
					{
						LevelEditor::RemoveComponents("Ui", name, &manager, 8, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Ui", name);
					}
					if (ImGui::Button("Remove Combat"))
					{
						ImGui::OpenPopup("Remove Combat?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<Combat>(ID))
					{
						LevelEditor::RemoveComponents("Combat", name, &manager, 9, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Combat", name);
					}
					if (ImGui::Button("Remove Enemy"))
					{
						ImGui::OpenPopup("Remove Enemy?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<Enemy>(ID))
					{
						LevelEditor::RemoveComponents("Enemy", name, &manager, 10, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Enemy", name);
					}
					if (ImGui::Button("Remove Logic"))
					{
						ImGui::OpenPopup("Remove Logic?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<Logic>(ID))
					{
						LevelEditor::RemoveComponents("Logic", name, &manager, 11, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Logic", name);
					}
					if (ImGui::Button("Remove Emitter"))
					{
						ImGui::OpenPopup("Remove Emitter?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<Emitter>(ID))
					{
						LevelEditor::RemoveComponents("Emitter", name, &manager, 12, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Emitter", name);
					}
					if (ImGui::Button("Remove Projectile"))
					{
						ImGui::OpenPopup("Remove Projectile?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<Projectile>(ID))
					{
						LevelEditor::RemoveComponents("Projectile", name, &manager, 13, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Projectile", name);
					}
					if (ImGui::Button("Remove Button"))
					{
						ImGui::OpenPopup("Remove Button?");
					}
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					if (manager.HasComponent<Projectile>(ID))
					{
						LevelEditor::RemoveComponents("Button", name, &manager, 15, ID);
					}
					else
					{
						LevelEditor::FailedRemoveComponents("Button", name);
					}
					ImGui::EndPopup();
			}
			if (scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);
			ImGui::PopID();
		}
		ImGui::PopID();
		name = "Object" + std::to_string(ID);
		ImGui::PopStyleColor();
	}
	ImGui::EndChild();
}

	void UiData::Gui_Objects(bool* , World& world)
	{
		World* tempW = &(world);
		//time_t now = time(nullptr);
		//char* dt = ctime(&now);
		static int counter = 0;
		int ID = 0;
		static ImGui_LayOut::AppLog log;
		std::string name = "Object" + std::to_string(ID);
		auto& manager = tempW->GetComponentManager();
		auto lookup = manager.GetComponentLookup<Sprite>();
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
		if (ImGui::TreeNode("Objects"))
		{
			static bool show_log = false;
			static bool obj_data = false;
			static bool scroll = false;
			ImGui::Checkbox("Scroll", &scroll);
			ImGui::Checkbox("Show Data Log", &show_log);
			ImGui::Checkbox("Show Obj Data", &obj_data);
			if (show_log)
			{
				Gui_Data_Info_Debug(world);
			}
			if (obj_data)
			{
				Gui_Datas(*tempW, scroll);
			}
			ImGui::TreePop();
		}
	}

	void UiData::Gui_Data_Info_Debug(World& world)
	{
		World* tempW = &(world);
		unsigned int ID = 1;
		std::string name = "Object" + std::to_string(ID);
		auto& manager = tempW->GetComponentManager();
		auto lookup = manager.GetComponentLookup<IsActive>();
		ImGui::Begin("Obj Debug");
		for (auto& pair : lookup)
		{
			auto& _transform = manager.GetComponent<Transform>(pair.first).get();
			ImGui::PushID(ID);
			auto _HasRigidBody = manager.GetComponent<RigidBody>(pair.first).has();
			if(_HasRigidBody)
			{
				auto& _RigidBody = manager.GetComponent<RigidBody>(pair.first).get();
				std::string DataScale{" Scale " + std::to_string(_transform.scale.x) + " , " + std::to_string(_transform.scale.y) + ")\n" };
				std::string DataPos{" Pos " + std::to_string(_transform.position.x) + " , " + std::to_string(_transform.position.y) + ")\n" };
				std::string Force{" Force X: " + std::to_string(_RigidBody.force.x) + " , " + "Force Y: " + std::to_string(_RigidBody.force.y) + ")\n" };
				std::string Vel{" Vel X: " + std::to_string(_RigidBody.velocity) + ")\n" };
				std::string VelDir{" Vel Dir X: " + std::to_string(_RigidBody.velocitydirection.x) + " , " + "Vel Dir Y: " + std::to_string(_RigidBody.velocitydirection.y) + ")\n" };
				std::string Mass{" Mass: " + std::to_string(_RigidBody.mass) + ")\n" };
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s: %s", name.c_str(), DataScale.c_str());
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.5f, 1.0f), "%s: %s", name.c_str(), DataPos.c_str());
				ImGui::TextColored(ImVec4(0.4f, 1.0f, 1.0f, 1.0f), "%s: %s",name.c_str(), Force.c_str());
				ImGui::TextColored(ImVec4(0.6f, 0.5f, 1.0f, 1.0f), "%s: %s", name.c_str(), Vel.c_str());
				ImGui::TextColored(ImVec4(0.9f, 1.0f, 0.5f, 1.0f), "%s: %s", name.c_str(), VelDir.c_str());
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s: %s", name.c_str(), Mass.c_str());
				ImGui_LayOut::Gui_InsertSeparator(1);
				
			}
			ImGui::PopID();
			++ID;
			name = UiAddName::GetName(ID);
		}
		ImGui::End();
	}
