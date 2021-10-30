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

		if (!engine->GetPlayState())
		{
			Entity entSelected = engine->editorManager->GetSelectedEntity();
			if (entSelected == MAX_ENTITY) return;

			if (!engine->world.CheckComponent<ParentComponent>(entSelected)) return;
			ParentComponent& parentComp = engine->world.GetComponent<ParentComponent>(entSelected);

			for (auto& childEntity : parentComp.child)
			{
				UpdateChildPosition(entSelected, childEntity);
			}
			
			return;
		}

		for (auto& entity : mEntities)
		{
			ParentComponent& parentComp = engine->world.GetComponent<ParentComponent>(entity);

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

		glm::mat4 T = glm::mat4(1.0f);
		glm::mat4 R = glm::mat4(1.0f);
		glm::mat4 identityMatrix = glm::mat4(1.0f);
		T = glm::translate(T, parentTransComp.position.ConvertToGlmVec3Type());
		R = glm::rotate(R, glm::radians(parentTransComp.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
		R = glm::rotate(R, glm::radians(parentTransComp.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
		R = glm::rotate(R, glm::radians(parentTransComp.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 model = T * R;
		ParentComponent& parentComp = engine->world.GetComponent<ParentComponent>(parentEnt);
		parentComp.model = model;

		model = glm::translate(model, childComp.PosOffset.ConvertToGlmVec3Type());
		glm::vec4 temp = glm::vec4{ 0, 0, 0, 1 };
		glm::vec3 newPos = model * temp;
		childTransComp.position = newPos;

		//std::cout << childComp.PosOffset.x << " " << childComp.PosOffset.y << " " << childComp.PosOffset.z << std::endl;

		childTransComp.rotation = parentTransComp.rotation + childComp.RotOffset;
		childTransComp.scale.setX(parentTransComp.scale.getX() * childComp.ScaleOffset.getX());
		childTransComp.scale.setY(parentTransComp.scale.getY() * childComp.ScaleOffset.getY());
		childTransComp.scale.setZ(parentTransComp.scale.getZ() * childComp.ScaleOffset.getZ());
	}
}