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
				m_ID, all_component_list);

			if (oldID != engine->gCamera.GetEditorCameraID() &&
				oldID != engine->gCamera.GetGameCameraID())
			{
				engine->gDynamicAABBTree.RemoveData(oldID);
				engine->gCullingManager->Remove(oldID);
				engine->LightManager.DestroyLight(oldID);
				engine->gPhysics.RemoveActor(oldID);
				engine->editorManager->DestroyEntity(oldID);
				engine->gPicker.SetCurrentCollisionID(engine->editorManager->GetSelectedEntity());
			}
		}

		virtual void Undo() override
		{
			Entity oldID = m_ID;
			m_ID = engine->prefabWorld.CopyEntity(engine->world,
				m_ID, all_component_list);

			auto& trans = engine->world.GetComponent<TransformComponent>(m_ID);
			auto& ent = engine->world.GetComponent<EntityComponent>(m_ID);
			
			if (ent.Tag == EntityType::ENT_MODEL)
			{
				auto& mesh = engine->world.GetComponent<MeshComponent>(m_ID);
				engine->AssimpManager.RegisterExistingModel(m_ID, mesh.MeshName.data());
			}
			else if (ent.Tag == EntityType::ENT_TARGETPOINT)
			{
				engine->gAI.AddTargetPointEntity(m_ID);
			}

			if (engine->world.CheckComponent<AABBComponent>(m_ID))
				engine->gPicker.GenerateAabb(m_ID, trans, ent.Tag);

			engine->editorManager->RegisterExistingEntity(m_ID);
			engine->prefabWorld.DestroyEntity(oldID);
		}

		virtual bool MergeCmds(ICommand* otherCmd) override
		{
			(void)otherCmd;
			return false;
		}

	private:
		Entity m_ID;
	};
}