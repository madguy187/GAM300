#pragma once
#include "Global.h"
#include "ComponentManager/ComponentManager.h"
#include "EntityManager/EntityManager.h"
#include "SystemManager/SystemManager.h"
#include "TypeID/TypeID.h"

namespace Eclipse
{
	class World
	{
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<SystemManager> systemManager;

	public:
		World();

		template <typename T>
		void RegisterComponent()
		{
			componentManager->RegisterComponent<T>();
		}

		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			entityManager->AddComponent(entity, componentManager->GetComponentType<T>());
			componentManager->AddComponent<T>(entity, component);
		}

		template <typename T>
		void DestroyComponent(Entity entity)
		{
			componentManager->RemoveComponent<T>(entity);
		}

		/*template <typename T>
		T& GetComponent(Entity entity)
		{
			return componentManager->GetComponent<T>(entity);
		}*/

		template <typename T>
		ComponentType GetComponentType()
		{
			return componentManager->GetComponentType<T>();
		}

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		
		void Clear();
	};
}