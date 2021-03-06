#pragma once
#include "Global.h"
#include "ComponentManager/ComponentManager.h"
#include "EntityManager/EntityManager.h"
#include "SystemManager/SystemManager.h"
#include "TypeID/TypeID.h"

//#include "Events/Events.h"

#include "ECS/ComponentManager/ComponentList.h"

namespace Eclipse
{
	class World
	{
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<SystemManager> systemManager;

	public:
		World();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void Clear();
		std::vector<std::string> GetAllComponentNames();

		template <typename T>
		void RegisterComponent()
		{
			componentManager->RegisterComponent<T>();
		}

		template <typename T>
		void RegisterSystem()
		{
			systemManager->RegisterSystem<T>();
		}

		template <typename T>
		void RegisterSystemSignature(Signature signature)
		{
			systemManager->SetSignature<T>(signature);
		}

		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			entityManager->AddComponent(entity, componentManager->GetComponentType<T>());
			componentManager->AddComponent<T>(entity, component);

			auto signature = entityManager->GetSignature(entity);
			signature.set(componentManager->GetComponentType<T>(), true);
			systemManager->EntitySignatureChanged(entity, signature);
		}

		template <typename T>
		void DestroyComponent(Entity entity)
		{
			entityManager->RemoveComponent(entity, componentManager->GetComponentType<T>());
			componentManager->RemoveComponent<T>(entity);

			auto signature = entityManager->GetSignature(entity);
			signature.set(componentManager->GetComponentType<T>(), false);
			systemManager->EntitySignatureChanged(entity, signature);
		}

		template <typename T>
		bool CheckComponent(Entity entity)
		{
			if (entity == MAX_ENTITY)
				return false;

			auto signature = entityManager->GetSignature(entity);

			int index = componentManager->GetComponentType<T>();
			if (signature[index])
				return true;

			return false;
		}

		template <typename T>
		T& GetComponent(Entity entity)
		{
			return componentManager->GetComponent<T>(entity);
		}

		template <typename T>
		ComponentType GetComponentType()
		{
			return componentManager->GetComponentType<T>();
		}

		template <typename T>
		void Update()
		{
			systemManager->Update<T>();
		}

		template <typename T>
		std::shared_ptr<T> GetSystem()
		{
			return systemManager->GetSystem<T>();
		}

		bool TempFunc(const Test1& t)
		{
			std::cout << "World.h TempFunc" << std::endl;
			(void)t;
			return false;
		}

		template <typename ...T>
		Entity CopyEntity(World& dest, Entity oldEnt, TypeList<T...>)
		{
			Entity newEnt = dest.CreateEntity();
			((CopyEntityComponent<T>(dest, newEnt, oldEnt)), ...);
			return newEnt;
		}

		template <typename T>
		void CopyEntityComponent(World& world, Entity newEnt, Entity oldEnt)
		{
			//std::cout << typeid(T).name() << std::endl;
			if (CheckComponent<T>(oldEnt))
			{
				world.AddComponent<T>(newEnt, GetComponent<T>(oldEnt));
			}
		}
	};
}