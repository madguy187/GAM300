#include "pch.h"
#include "ParentSystem.h"
#include "Global.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"

namespace Eclipse
{
	void ParentSystem::Update()
	{
		ZoneScopedN("Parent System")
		engine->Timer.SetName({ SystemName::PARENT });
		engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

		for (auto& entity : mEntities)
		{
			ParentComponent& parentComp = engine->world.GetComponent<ParentComponent>(entity);

			//for (auto& childEntity : parentComp.child)
			for (auto& childEntity : parentComp.child)
			{
				UpdateChildPosition(entity, childEntity);
			}
		}

		engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
		FrameMark
	}

	void ParentSystem::UpdateChildPosition(Entity parentEnt, Entity childEnt)
	{
		ChildComponent& childComp = engine->world.GetComponent<ChildComponent>(childEnt);
		TransformComponent& childTransComp = engine->world.GetComponent<TransformComponent>(childEnt);
		TransformComponent& parentTransComp = engine->world.GetComponent<TransformComponent>(parentEnt);

		ECVec3 Zero{ 0, 0, 0 };
		if (childComp.PosOffset == Zero)
			childComp.PosOffset = parentTransComp.position - childTransComp.position;
		if (childComp.RotOffset == Zero)
			childComp.RotOffset = parentTransComp.rotation - childTransComp.position;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, childComp.PosOffset.ConvertToGlmVec3Type() + parentTransComp.position.ConvertToGlmVec3Type());
		model = model / glm::determinant(model); // normalize
		model = glm::rotate(model, glm::radians(childComp.RotOffset.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(childComp.RotOffset.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(childComp.RotOffset.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
		
		parentTransComp.position = model * parentTransComp.position.ConvertToGlmVec4Type();
	}
}