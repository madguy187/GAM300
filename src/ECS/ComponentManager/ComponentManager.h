#pragma once
#include "Global.h"
#include "Components/ComponentArray/ComponentArray.h"

namespace Eclipse
{
	class ComponentManager
	{
	public:
		template<typename... T>
		void RegisterComponent()
		{
			
			const char* typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

			// Add this component type to the component type map
			mComponentTypes.insert({ typeName, mNextComponentType });

			// Create a ComponentArray pointer and add it to the component arrays map
			mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

			// Increment the value so that the next component registered will be different
			++mNextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

			// Return this component's type - used for creating signatures
			return mComponentTypes[typeName];
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			// Add a component to the array for an entity
			GetComponentArray<T>()->Insert(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			// Remove a component from the array for an entity
			GetComponentArray<T>()->Delete(entity);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			// Get a reference to a component from the array for an entity
			return GetComponentArray<T>()->GetComponent(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			/*for (auto const& pair : mComponentArrays)
			{
				auto const& component = pair.second;

				component->Delete(entity);
			}*/
		}

	private:
		std::unordered_map<const char*, ComponentType> mComponentTypes{};
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

		ComponentType mNextComponentType{};

		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
		}
	};
}