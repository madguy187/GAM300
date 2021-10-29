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

		/*Entity entSelected = engine->editorManager->GetSelectedEntity();
		if (entSelected == MAX_ENTITY) return;
		if (!engine->world.CheckComponent<ChildComponent>(entSelected)) return;*/

		for (auto entity : mEntities)
		{
			ChildComponent& childComp = engine->world.GetComponent<ChildComponent>(entity);
			if (!childComp.UpdateChildren) return;

			TransformComponent& childTrans = engine->world.GetComponent<TransformComponent>(entity);
			TransformComponent& parentTrans = engine->world.GetComponent<TransformComponent>(childComp.parentIndex);

			/*float distance = abs(VectorDistance<float, 3>(childTrans.position, parentTrans.position));
			if (abs(distance - childComp.distance) < 0.1) return;*/

			/*float difference = abs(distance - childComp.distance);*/
			childComp.PosOffset = childTrans.position - parentTrans.position;
			childComp.RotOffset = childTrans.rotation - parentTrans.rotation;

			std::cout << childComp.PosOffset.x << " " << childComp.PosOffset.y << " " << childComp.PosOffset.z << std::endl;
		}

		engine->Timer.SetName({ SystemName::CHILD });
		engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());
		engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
		FrameMark
	}
}
