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

		for (auto entity : mEntities)
		{
			ChildComponent& childComp = engine->world.GetComponent<ChildComponent>(entity);
			if (!childComp.UpdateChildren) return;

			TransformComponent& childTrans = engine->world.GetComponent<TransformComponent>(entity);
			TransformComponent& parentTrans = engine->world.GetComponent<TransformComponent>(childComp.parentIndex);

			ParentComponent& parentComp = engine->world.GetComponent<ParentComponent>(childComp.parentIndex);
			glm::mat4 model = glm::inverse(parentComp.model);

			glm::vec4 oldParentPos = glm::vec4{ parentTrans.position.ConvertToGlmVec3Type(), 1 };
			glm::vec4 oldChildPos = glm::vec4{ childTrans.position.ConvertToGlmVec3Type(), 1 };
			glm::vec3 parentPos = model * oldParentPos;
			glm::vec3 childPos = model * oldChildPos;

			childComp.PosOffset = childPos - parentPos;
			childComp.RotOffset = childTrans.rotation - parentTrans.rotation;
		}

		engine->Timer.SetName({ SystemName::CHILD });
		engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());
		engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
		FrameMark
	}
}
