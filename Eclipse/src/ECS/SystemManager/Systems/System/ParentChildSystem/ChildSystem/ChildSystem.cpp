#include "pch.h"
#include "ChildSystem.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"

namespace Eclipse
{
	void Eclipse::ChildSystem::Update()
	{
		ZoneScopedN("Child System")
		if (engine->GetPlayState()) return;

		//int gizmoType = engine->editorManager->GetEditorWindow<SceneWindow>()->GetGizmoType();
		//if (gizmoType != ImGuizmo::OPERATION::TRANSLATE) return;

		for (auto& entity : mEntities)
		{
			ChildComponent& childComp = engine->world.GetComponent<ChildComponent>(entity);
			TransformComponent& childTrans = engine->world.GetComponent<TransformComponent>(entity);
			TransformComponent& parentTrans = engine->world.GetComponent<TransformComponent>(childComp.parentIndex);

			childComp.PosOffset.x = abs(parentTrans.position.x - childTrans.position.x);
			childComp.PosOffset.y = abs(parentTrans.position.y - childTrans.position.y);
			childComp.PosOffset.z = abs(parentTrans.position.z - childTrans.position.z);
		}

		engine->Timer.SetName({ SystemName::CHILD });
		engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());
		engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
		FrameMark
	}
}
