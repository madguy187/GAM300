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
					MyCleanUp(oldID);
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

				if (engine->world.CheckComponent<ChildComponent>(m_ID))
				{
					auto& child = engine->world.GetComponent<ChildComponent>(m_ID);
					auto& parent = engine->world.GetComponent<ParentComponent>(child.parentIndex);
					parent.child.push_back(m_ID);
				}
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
			std::set<Entity> childrenToBeDeleted;
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

						if (engine->world.CheckComponent<ChildComponent>(child) 
							&& !engine->world.CheckComponent<ParentComponent>(child))
						{
							childrenToBeDeleted.insert(child);
						}
					}

					for (const auto& byeChild : childrenToBeDeleted)
					{
						auto& childByeCom = engine->world.GetComponent<ChildComponent>(byeChild);
						auto& parentLoser = engine->world.GetComponent<ParentComponent>(childByeCom.parentIndex);
						parentLoser.child.erase(std::find(parentLoser.child.begin(), parentLoser.child.end(), byeChild));
						engine->CleanUp(byeChild);
					}

					childrenToBeDeleted.clear();
					ParentToFamily[OldToNewEntity[CurrID]] = List;
				}
			}

			for (const auto& parent : parentsToBeDeleted)
				engine->CleanUp(parent);
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

		void MyCleanUp(Entity ID)
		{
			if (engine->world.CheckComponent<ChildComponent>(ID))
			{
				auto& child = engine->world.GetComponent<ChildComponent>(ID);
				auto& parent = engine->world.GetComponent<ParentComponent>(child.parentIndex);
				parent.child.erase(std::find(parent.child.begin(), parent.child.end(), ID));
			}

			engine->CleanUp(ID);
		}
	private:
		Entity m_ID;
		std::vector<Entity> Children;
		std::vector<Entity> m_pcEntities;
		std::unordered_map<Entity, std::vector<Entity>> ParentToFamily;
	};
}