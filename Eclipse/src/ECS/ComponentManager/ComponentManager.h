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
				},
				[](IComponentArray& _container, const Entity entity)
				{
					static_cast<ComponentArray<T>&>(_container).GetComponent(entity);
				},
				[](IComponentArray& _container)
				{
					static_cast<ComponentArray<T>&>(_container).Clear();
				},
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
			auto& base = static_cast<ComponentArray<T>&>(*(mComponentArrays[id.GetID<T>()]._container));
			base.Insert(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			// Remove a component from the array for an entity
			ComponentBase& base = mComponentArrays[id.GetID<T>()];
			base.Delete(*base._container, entity);
		}

		void RemoveComponentByID(Entity entity, size_t index)
		{
			// Remove a component from the array for an entity
			ComponentBase& base = mComponentArrays[index];
			base.Delete(*base._container, entity);
		}

		void RemoveComponentBySignature(Entity entity, Signature signature)
		{
			for (size_t i = 0; i < signature.size(); i++)
			{
				if (signature[i])
				{
					ComponentBase& base = mComponentArrays[i];
					base.Delete(*base._container, entity);
				}
			}
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			// Get a reference to a component from the array for an entity
			auto& base = static_cast<ComponentArray<T>&>(*(mComponentArrays[id.GetID<T>()]._container));
			return base.GetComponent(entity);
		}

		void Clear();

	private:
		std::vector<ComponentBase> mComponentArrays;
		TypeID<IComponentArray> id;

		template<typename T>
		std::unique_ptr<ComponentArray<T>> GetComponentArray()
		{
			return mComponentArrays[id.GetID<T>()];
		}
	};
}