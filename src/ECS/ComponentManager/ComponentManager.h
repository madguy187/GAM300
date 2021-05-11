#pragma once
#include "Global.h"
#include "Components/ComponentArray/ComponentBase.h"
#include "Components/ComponentArray/ComponentArray.h"
#include "TypeID/TypeID.h"

unsigned TypeID<Eclipse::IComponentArray>::_counter = 0;

namespace Eclipse
{
	class ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			// Create a ComponentArray pointer and add it to the component arrays map
			mComponentArrays.push_back({ std::make_unique<ComponentArray<T>>(),
				[](IComponentArray& _container, const Entity entity)
				{ 
					static_cast<ComponentArray<T>&>(_container).Delete(entity);
				}
			});

			id.RegisterID<T>();
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return id.GetID<T>();
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			// Add a component to the array for an entity
			//GetComponentArray<T>()->Insert(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			// Remove a component from the array for an entity
			ComponentBase& base = mComponentArrays[id.GetID<T>()];
			base.Delete(base._container, entity);
		}

		//template<typename T>
		//T& GetComponent(Entity entity)
		//{
		//	// Get a reference to a component from the array for an entity
		//	return GetComponentArray<T>()->GetComponent(entity);
		//}

		void Clear();

		/*void EntityDestroyed(Entity entity)
		{
			for (int i = 0; i < mComponentArrays.size(); i++)
			{

			}

			for (auto const& pointer : mComponentArrays)
			{
				std::static_pointer_cast<ComponentArray<T>>(pointer)->Delete(entity);
			}
		}*/

	private:
		std::vector<ComponentBase> mComponentArrays;
		TypeID<IComponentArray> id;

		/*template<typename T>
		std::unique_ptr<ComponentArray<T>> GetComponentArray()
		{
			return mComponentArrays[id.GetID<T>()];
		}*/
	};
}