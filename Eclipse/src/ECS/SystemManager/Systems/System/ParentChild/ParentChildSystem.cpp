#include "pch.h"
#include "ParentChildSystem.h"
#include "ECS/ComponentManager/Components/ParentChildComponent.h"

namespace Transcend
{
	void ParentChildSystem::Update()
	{
		//for (auto const& entity : entities)
		//{
		//	if (engine->GetPlayState() && !engine->GetPauseState())
		//	{
		//		ParentChildComponent& childComponent = engine->coordinator.getComponent<ParentChildComponent>(entity);
		//		if (childComponent.parentIndex != MAX_ENTITY + 1 && std::find(engine->_EntityList.begin(), engine->_EntityList.end(), childComponent.parentIndex) == engine->_EntityList.end())
		//		{
		//			childComponent.parentIndex = MAX_ENTITY + 1;
		//			engine->_ddo.DestroyEntity(entity);
		//			continue;
		//		}
		//		if (childComponent.parentIndex == MAX_ENTITY + 1) continue;

		//		TransformComponent& childTransform = engine->coordinator.getComponent<TransformComponent>(entity);
		//		// Player
		//		TransformComponent& parentTransform = engine->coordinator.getComponent<TransformComponent>(childComponent.parentIndex);
		//		childTransform.position = parentTransform.position + childComponent.offset;
		//	}
		//	else
		//	{
		//		ParentChildComponent& childComponent = engine->coordinator.getComponent<ParentChildComponent>(entity);
		//		if (childComponent.ForceOffset) continue;
		//		if (childComponent.parentIndex != MAX_ENTITY + 1 && std::find(engine->_EntityList.begin(), engine->_EntityList.end(), childComponent.parentIndex) == engine->_EntityList.end())
		//		{
		//			childComponent.parentIndex = MAX_ENTITY + 1;
		//			engine->_ddo.DestroyEntity(entity);
		//			continue;
		//		}
		//		if (childComponent.parentIndex == MAX_ENTITY + 1) continue;
		//		TransformComponent& childTransform = engine->coordinator.getComponent<TransformComponent>(entity);
		//		TransformComponent& parentTransform = engine->coordinator.getComponent<TransformComponent>(childComponent.parentIndex);

		//		/*if (parentTransform.position.getX() < childTransform.position.getX())
		//			childComponent.offset.setX(parentTransform.position.getX() - childTransform.position.getX());
		//		else*/
		//		childComponent.offset.setX(childTransform.position.getX() - parentTransform.position.getX());

		//		/*if (parentTransform.position.getY() < childTransform.position.getY())
		//			childComponent.offset.setY(parentTransform.position.getY() - childTransform.position.getY());
		//		else*/
		//		childComponent.offset.setY(childTransform.position.getY() - parentTransform.position.getY());
		//	}
		//}
	}

	void ParentChildSystem::PostUpdate()
	{
		/*for (auto const& entity : entities)
		{
			ParentChildComponent& childComponent = engine->coordinator.getComponent<ParentChildComponent>(entity);
			if (childComponent.parentIndex == MAX_ENTITY + 1) continue;

			if (engine->coordinator.checkComponent(entity, engine->coordinator.getComponentBinary<TransformComponent>()))
			{
				TransformComponent& childTransform = engine->coordinator.getComponent<TransformComponent>(entity);
				TransformComponent& parentTransform = engine->coordinator.getComponent<TransformComponent>(childComponent.parentIndex);

				childTransform.position = parentTransform.position + childComponent.offset;
			}
		}*/
	}
}