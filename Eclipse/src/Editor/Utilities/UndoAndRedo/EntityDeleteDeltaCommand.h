#pragma once

#include "ICommand.h"

namespace Eclipse
{
	class EntityDeleteDeltaCommand : public ICommand
	{
	public:
		EntityDeleteDeltaCommand(Entity& ID) :
			m_ID{ ID }
		{}

		virtual void Execute() override
		{
			Entity oldID = m_ID;
			m_ID = engine->world.CopyEntity(engine->prefabWorld,
				oldID, all_component_list);

			if (oldID != engine->gCamera.GetEditorCameraID() &&
				oldID != engine->gCamera.GetGameCameraID())
			{
				if (engine->world.CheckComponent<ParentComponent>(oldID))
				{
					auto& parent = engine->world.GetComponent<ParentComponent>(oldID);
					std::vector<Entity> List;

					for (const auto& child : parent.child)
					{
						Entity newChildID = engine->world.CopyEntity(engine->prefabWorld,
							child, all_component_list);
						List.push_back(newChildID);

						CleanUp(child);
					}

					Children = List;
				}
				else if (engine->world.CheckComponent<ChildComponent>(oldID))
				{
					auto& child = engine->world.GetComponent<ChildComponent>(oldID);
					auto& parent = engine->world.GetComponent<ParentComponent>(child.parentIndex);

					parent.child.erase(std::find(parent.child.begin(), parent.child.end(), oldID));

					auto& parentEntComp = engine->world.GetComponent<EntityComponent>(child.parentIndex);
					parentEntComp.Child.erase(std::find(parentEntComp.Child.begin(), parentEntComp.Child.end(), oldID));
				}

				CleanUp(oldID);
			}
		}

		virtual void Undo() override
		{
			Entity oldID = m_ID;
			m_ID = engine->prefabWorld.CopyEntity(engine->world,
				oldID, all_component_list);

			LoadIn(m_ID, oldID);

			if (engine->world.CheckComponent<ParentComponent>(m_ID))
			{
				auto& parent = engine->world.GetComponent<ParentComponent>(m_ID);
				parent.child.clear();

				auto& parentEntComp = engine->world.GetComponent<EntityComponent>(m_ID);
				parentEntComp.Child.clear();

				for (const auto& child : Children)
				{
					Entity newChild = engine->prefabWorld.CopyEntity(engine->world,
						child, all_component_list);
					auto& childCom = engine->world.GetComponent<ChildComponent>(newChild);
					auto& childEntComp = engine->world.GetComponent<EntityComponent>(newChild);

					childCom.parentIndex = m_ID;
					childEntComp.Parent.clear();
					childEntComp.Parent.push_back(m_ID);
					parent.child.push_back(newChild);
					parentEntComp.Child.push_back(newChild);
					LoadIn(newChild, child);
				}
			}
			else if (engine->world.CheckComponent<ChildComponent>(m_ID))
			{
				auto& child = engine->world.GetComponent<ChildComponent>(m_ID);
				auto& parent = engine->world.GetComponent<ParentComponent>(child.parentIndex);

				parent.child.push_back(m_ID);

				auto& parentEntComp = engine->world.GetComponent<EntityComponent>(child.parentIndex);
				parentEntComp.Child.push_back(m_ID);
			}
		}

		virtual bool MergeCmds(ICommand* otherCmd) override
		{
			(void)otherCmd;
			return false;
		}

		void LoadIn(Entity newID, Entity oldID)
		{
			auto& trans = engine->world.GetComponent<TransformComponent>(newID);
			auto& ent = engine->world.GetComponent<EntityComponent>(newID);

			if (ent.Tag == EntityType::ENT_MODEL)
			{
				engine->AssimpManager.RegisterExistingModel(newID);
			}
			else if (ent.Tag == EntityType::ENT_MESH)
			{
				auto& mesh = engine->world.GetComponent<MeshComponent>(newID);
				engine->AssimpManager.RegisterExistingModel(newID, mesh.MeshName.data());
			}
			else if (ent.Tag == EntityType::ENT_TARGETPOINT)
			{
				engine->gAI.AddTargetPointEntity(newID);
			}

			if (engine->world.CheckComponent<AABBComponent>(newID))
				engine->gPicker.GenerateAabb(newID, trans, ent.Tag);

			engine->editorManager->RegisterExistingEntity(newID);
			engine->prefabWorld.DestroyEntity(oldID);
		}

		void CleanUp(Entity ID)
		{
			engine->gDynamicAABBTree.RemoveData(ID);
			engine->gCullingManager->Remove(ID);
			engine->LightManager.DestroyLight(ID);
			engine->gPhysics.RemoveActor(ID);
			engine->editorManager->DestroyEntity(ID);
			engine->gPicker.SetCurrentCollisionID(engine->editorManager->GetSelectedEntity());
		}
	private:
		Entity m_ID;
		std::vector<Entity> Children;
	};
}