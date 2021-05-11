#pragma once
#include "pch.h"
#include "ComponentArray.h"

namespace Eclipse
{
	struct ComponentBase
	{
		std::unique_ptr<IComponentArray> _container;
		void (*Delete)(IComponentArray&, const Entity);
	};
}


/*template<typename Component>
void RegisterComponent(const char* component_name) {
	unsigned index = TypeID<World>::GetID<Component>();
	HU_ASSERT_LVL_0(index <= m_component_containers.size(), "some registered components not in container");
	if (index == m_component_containers.size()) 
	{ 
		m_component_containers.push_back({ 
			std::make_unique<ComponentHandler<Component>>(), [](SparseSet& container, World& world, const Entity entity) 
			{
				static_cast<ComponentHandler<Component>&>(container).Remove(world,entity); }, [](SparseSet& container, Serializer& serializer, const Entity entity, const char* component_name) {
			static_cast<ComponentHandler<Component>&>(container).SerializeData(serializer, entity, component_name); }, [](SparseSet& container, World& world, const Entity entity, ObjectFileType& object) {
				static_cast<ComponentHandler<Component>&>(container).UnserializeData(world, entity, object); }, [](SparseSet& container, World& dest_world, const Entity dest_entity, const Entity src_entity) { 
					static_cast<ComponentHandler<Component>&>(container).Clone(dest_world, dest_entity, src_entity); }, [](SparseSet& container, World& dest_world, const Entity dest_entity, const Entity src_entity) { 
						static_cast<ComponentHandler<Component>&>(container).UpdateClone(dest_world, dest_entity, src_entity); }, 
						[](SparseSet& container, World& world) { 
							ComponentHandler<Component>& comp_container = static_cast<ComponentHandler<Component>&>(container); 
							comp_container.Each([&comp_container, &world](Entity e) 
							{
								comp_container.Remove(world, e); 
							}); } 
			}); 
	}*/            //add names and free list for registerd component            m_component_names.push_back(component_name);            m_component_hash.push_back(HashString::Value(component_name));            m_component_free_list.emplace_back();        }