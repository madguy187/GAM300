#pragma once
#include "pch.h"
#include "ComponentArray.h"

namespace Eclipse
{
	struct ComponentBase
	{
		std::unique_ptr<IComponentArray> _container;
		void (*Delete)(IComponentArray&, const Entity);
		void (*GetComponent)(IComponentArray&, const Entity);
		void (*Clear)(IComponentArray&);
	};
}