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
			/*EntityComponent& entityComp = engine->world.GetComponent<EntityComponent>(entity);
			if (entityComp.Tag != EntityType::ENT_MODEL || !engine->GetPlayState()) continue;*/
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

		//glm::mat4 translate1 = glm::mat4(1.0f);
		//glm::mat4 translate2 = glm::mat4(1.0f);
		//glm::mat4 rotate = glm::mat4(1.0f);
		//translate1 = glm::translate(translate1, parentTransComp.position.ConvertToGlmVec3Type());
		//rotate = translate1 * rotate;
		//rotate = glm::rotate(rotate, glm::radians(parentTransComp.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
		//rotate = glm::rotate(rotate, glm::radians(parentTransComp.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
		//rotate = glm::rotate(rotate, glm::radians(parentTransComp.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
		////translate2 = rotate * translate2;
		//rotate = glm::translate(rotate, childComp.PosOffset.ConvertToGlmVec3Type());

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 identityMatrix = glm::mat4(1.0f);
		model = glm::translate(model, parentTransComp.position.ConvertToGlmVec3Type());
		model = model * identityMatrix;
		model = glm::rotate(model, glm::radians(parentTransComp.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(parentTransComp.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(parentTransComp.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = model * identityMatrix;

		std::cout << model[3][0] << std::endl;
		std::cout << model[3][1] << std::endl;
		std::cout << model[3][2] << std::endl;
		std::cout << model[3][3] << std::endl;
		std::cout << std::endl;

		model = glm::translate(model, childComp.PosOffset.ConvertToGlmVec3Type());
		glm::vec4 temp = glm::vec4{ 0, 0, 0, 1 };
		glm::vec3 newPos = model * temp;
		childTransComp.position = newPos;
		childTransComp.rotation = parentTransComp.rotation + childComp.RotOffset;
	}
}