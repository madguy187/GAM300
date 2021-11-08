#pragma once

#include "ECS/ComponentManager/Components/ScriptComponent.h"
#include "ECS/ComponentManager/Components/AabbComponent.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"
#include "ECS/ComponentManager/Components/DirectionalLightComponent.h"
#include "ECS/ComponentManager/Components/PointLightComponent.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/LightComponent.h"
#include "ECS/ComponentManager/Components/MaterialComponent.h"
#include "ECS/ComponentManager/Components/MeshComponent.h"
#include "ECS/ComponentManager/Components/ModelComponent.h"
#include "ECS/ComponentManager/Components/AIComponent.h"
#include "ECS/ComponentManager/Components/SpotLightComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/PrefabComponent.h"
#include "ECS/ComponentManager/Components/AudioComponent.h"
#include "ECS/ComponentManager/Components/RigidBodyComponent.h"
#include "ECS/ComponentManager/Components/CollisionComponent.h"
#include "ECS/ComponentManager/Components/AnimationComponent.h"

namespace Eclipse
{
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
		PrefabComponent,
		AnimationComponent
	>;

	template <typename ...Types>
	constexpr ComponentTypeList<Types...> component_list{};
	
	constexpr ComponentList all_component_list{};
}