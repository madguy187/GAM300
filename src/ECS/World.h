#pragma once
#include "Global.h"
#include "ComponentManager/ComponentManager.h"
#include "EntityManager/EntityManager.h"
#include "SystemManager/SystemManager.h"

namespace Eclipse
{
	class World
	{
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<SystemManager> systemManager;
	public:
		World();

		template <typename... Types>
		void RegisterComponent();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		
		template <typename T>
		void AddComponent(Entity entity, T component);

		template <typename T>
		void DestroyComponent(Entity entity);

		void Clear();
	};
}