#pragma once
#include "Global.h"
#include "ComponentManager/ComponentManager.h"
#include "EntityManager/EntityManager.h"
#include "SystemManager/SystemManager.h"
#include "TypeID/TypeID.h"

//#include "Events/Events.h"

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
		T& GetComponent(Entity entity)
		{
			return componentManager->GetComponent<T>(entity);
		}

		template <typename T>
		ComponentType GetComponentType()
		{
			return componentManager->GetComponentType<T>();
		}

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		bool CheckComponent(Entity entity, Signature signature);
		
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

		void Clear();

		bool TempFunc(const Test1& t)
		{
			std::cout << "World.h TempFunc" << std::endl;
			return false;
		}
	};
}