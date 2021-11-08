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
					m_pcEntities.push_back(m_ID);
					IterativeDeletion(oldID, m_ID);
				}
				else
				{
					CleanUp(oldID);
				}
			}
		}

		virtual void Undo() override
		{
			Entity oldID = m_ID;
			m_ID = engine->prefabWorld.CopyEntity(engine->world,
				oldID, all_component_list);

			if (engine->prefabWorld.CheckComponent<ParentComponent>(oldID))
			{
				DeletionResolve(oldID, m_ID);
			}
			else
			{
				LoadIn(m_ID, oldID);
			}
		}

		virtual bool MergeCmds(ICommand* otherCmd) override
		{
			(void)otherCmd;
			return false;
		}

		void DeletionResolve(Entity oldID, Entity newID)
		{
			std::unordered_map<Entity, Entity> OldToNewEntity;
			OldToNewEntity[oldID] = newID;
			LoadIn(newID, oldID);
			// m_pcEntities.erase(std::find(m_pcEntities.begin(), m_pcEntities.end(), newID));

			for (const auto& oldEnt : m_pcEntities)
			{
				if (oldID == oldEnt) continue;

				Entity newEnt = engine->prefabWorld.CopyEntity(engine->world,
					oldEnt, all_component_list);

				LoadIn(newEnt, oldEnt);

				OldToNewEntity[oldEnt] = newEnt;
			}

			for (const auto& [parent, childList] : ParentToFamily)
			{
				auto& parentcom = engine->world.GetComponent<ParentComponent>(OldToNewEntity[parent]);
				parentcom.child.clear();

				for (const auto& child2 : childList)
				{
					parentcom.child.push_back(OldToNewEntity[child2]);

					auto& childcom = engine->world.GetComponent<ChildComponent>(OldToNewEntity[child2]);
					childcom.parentIndex = OldToNewEntity[parent];
				}
			}

			m_pcEntities.clear();
			ParentToFamily.clear();
		}

		void IterativeDeletion(Entity oldID, Entity newID)
		{
			std::queue<Entity> parents;
			std::set<Entity> parentsToBeDeleted;
			std::unordered_map<Entity, Entity> OldToNewEntity;
			OldToNewEntity[oldID] = newID;
			parents.push(oldID);

			while (!parents.empty())
			{
				Entity CurrID = parents.front();
				parentsToBeDeleted.insert(CurrID);
				parents.pop();

				if (engine->world.CheckComponent<ParentComponent>(CurrID))
				{
					auto& parent = engine->world.GetComponent<ParentComponent>(CurrID);
					std::vector<Entity> List;

					for (const auto& child : parent.child)
					{
						Entity newChildID = engine->world.CopyEntity(engine->prefabWorld,
							child, all_component_list);

						m_pcEntities.push_back(newChildID);

						if (engine->world.CheckComponent<ParentComponent>(child))
						{
							parents.push(child);
							OldToNewEntity[child] = newChildID;
						}

						List.push_back(newChildID);

						if (engine->world.CheckComponent<ChildComponent>(child))
						{
							auto& child2 = engine->world.GetComponent<ChildComponent>(child);
							auto& parent2 = engine->world.GetComponent<ParentComponent>(child2.parentIndex);

							if (!engine->world.CheckComponent<ParentComponent>(child)
								&& engine->world.CheckComponent<ChildComponent>(child))
								CleanUp(child);

							parent2.child.erase(std::find(parent2.child.begin(), parent2.child.end(), child));
						}
					}

					ParentToFamily[OldToNewEntity[CurrID]] = List;
				}
			}

			for (const auto& ent : parentsToBeDeleted)
				CleanUp(ent);
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
		std::vector<Entity> m_pcEntities;
		std::unordered_map<Entity, std::vector<Entity>> ParentToFamily;
	};
}