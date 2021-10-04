#pragma once

namespace Eclipse
{
	struct AABBComponent;
	struct CameraComponent;
	struct DirectionalLightComponent;
	struct EntityComponent;
	struct LightComponent;
	struct MaterialComponent;
	struct MeshComponent;
	struct ModeLInforComponent;
	struct PointLightComponent;
	struct RigidBodyComponent;
	struct ScriptComponent;
	struct ParentComponent;
	struct ChildComponent;
	struct CollisionComponent;
	struct AIComponent;
	struct SpotLightComponent;
	struct TransformComponent;
	struct PrefabComponent;

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
		CollisionComponent,
		AIComponent,
		SpotLightComponent,
		TransformComponent,
		PrefabComponent
	>;

	template <typename ...Types>
	constexpr ComponentTypeList<Types...> component_list{};
	
	constexpr ComponentList all_component_list{};
}