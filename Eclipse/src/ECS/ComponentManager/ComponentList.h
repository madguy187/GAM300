#pragma once
#include "ECS/ComponentManager/Components/ScriptComponent.h"
#include "ECS/ComponentManager/Components/AabbComponent.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"

namespace Eclipse
{
	struct CameraComponent;
	struct DirectionalLightComponent;
	struct EntityComponent;
	struct LightComponent;
	struct MaterialComponent;
	struct MeshComponent;
	struct ModelComponent;
	struct PointLightComponent;
	struct RigidBodyComponent;
	struct ParentComponent;
	struct ChildComponent;
	struct CollisionComponent;
	struct AIComponent;
	struct SpotLightComponent;
	struct TransformComponent;
	struct PrefabComponent;
	struct AudioComponent;

	template<typename ...T>
	struct TypeList
	{

	};

	template <typename ...T>
	struct ComponentTypeList : TypeList<T...>
	{

	};

	using ComponentList = ComponentTypeList<
		AABBComponent,
		CameraComponent,
		DirectionalLightComponent,
		EntityComponent,
		LightComponent,
		MaterialComponent,
		MeshComponent,
		ModelComponent,
		PointLightComponent,
		RigidBodyComponent,
		ScriptComponent,
		ParentComponent,
		ChildComponent,
		CollisionComponent,
		AudioComponent,
		AIComponent, 
		SpotLightComponent,
		TransformComponent,
		PrefabComponent
	>;

	template <typename ...Types>
	constexpr ComponentTypeList<Types...> component_list{};
	
	constexpr ComponentList all_component_list{};
}