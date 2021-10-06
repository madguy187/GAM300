#pragma once
#include "ECS/ComponentManager/Components/ScriptComponent.h"
#include "ECS/ComponentManager/Components/AabbComponent.h"

namespace Eclipse
{
	struct CameraComponent;
	struct DirectionalLightComponent;
	struct EntityComponent;
	struct LightComponent;
	struct MaterialComponent;
	struct MeshComponent;
	struct ModeLInforComponent;
	struct PointLightComponent;
	struct RigidBodyComponent;
	struct ParentComponent;
	struct ChildComponent;
	struct SpotLightComponent;
	struct TransformComponent;

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
		ModeLInforComponent,
		PointLightComponent,
		RigidBodyComponent,
		ScriptComponent,
		ParentComponent,
		ChildComponent,
		SpotLightComponent,
		TransformComponent
	>;

	template <typename ...Types>
	constexpr ComponentTypeList<Types...> component_list{};
	
	constexpr ComponentList all_component_list{};
}