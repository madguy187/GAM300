#pragma once

namespace Eclipse
{
	struct AABBComponent;
	struct CameraComponent;
	struct ChildTransformComponent;
	struct DirectionalLightComponent;
	struct EntityComponent;
	struct LightComponent;
	struct MaterialComponent;
	struct MeshComponent;
	struct ModeLInforComponent;
	struct ParentChildComponent;
	struct PointLightComponent;
	struct RigidBodyComponent;
	struct ScriptComponent;

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
		ChildTransformComponent,
		DirectionalLightComponent,
		EntityComponent,
		LightComponent,
		MaterialComponent,
		MeshComponent,
		ModeLInforComponent,
		ParentChildComponent,
		PointLightComponent,
		RigidBodyComponent,
		ScriptComponent
	>;

	template <typename ...Types>
	constexpr ComponentList component_list{};
}