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

	static void SetIntensity(Entity ent, float value)
	{
		if (engine->world.CheckComponent<PointLightComponent>(ent))
		{
			engine->world.GetComponent<PointLightComponent>(ent).IntensityStrength = value;
		}

		if (engine->world.CheckComponent<SpotLightComponent>(ent))
		{
			engine->world.GetComponent<SpotLightComponent>(ent).IntensityStrength = value;
		}
	}

	static void SetDirection(Entity ent, MonoObject* value)
	{
		if (engine->world.CheckComponent<SpotLightComponent>(ent))
		{
			ECVec3 Dir = engine->mono.ConvertVectorToECVec(value);
			engine->world.GetComponent<SpotLightComponent>(ent).direction = Dir;
		}
	}

	static void IncreaseIntensity(Entity ent, float value)
	{
		if (engine->world.CheckComponent<SpotLightComponent>(ent))
		{
			engine->world.GetComponent<SpotLightComponent>(ent).IntensityStrength += value;
		}
	}

	static void DecreaseIntensity(Entity ent, float value)
	{
		if (engine->world.CheckComponent<SpotLightComponent>(ent))
		{
			engine->world.GetComponent<SpotLightComponent>(ent).IntensityStrength = value;
		}
	}

	static float GetIntensity(Entity ent)
	{
		if (engine->world.CheckComponent<SpotLightComponent>(ent))
		{
			return engine->world.GetComponent<SpotLightComponent>(ent).IntensityStrength;
		}
	}
}