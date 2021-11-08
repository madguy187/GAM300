#pragma once
#include "pch.h"
#include "mono/jit/jit.h"

namespace Eclipse
{
	static void SetLightEnabled(Entity ent, int state)
	{
		if (engine->world.CheckComponent<DirectionalLightComponent>(ent))
		{
			engine->world.GetComponent<DirectionalLightComponent>(ent).AffectsWorld = state ? true : false;
		}

		if (engine->world.CheckComponent<PointLightComponent>(ent))
		{
			engine->world.GetComponent<PointLightComponent>(ent).AffectsWorld = state ? true : false;
		}

		if (engine->world.CheckComponent<SpotLightComponent>(ent))
		{
			engine->world.GetComponent<SpotLightComponent>(ent).AffectsWorld = state ? true : false;
		}
	}
}