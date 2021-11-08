#pragma once
#include "Global.h"
#include "pch.h"

namespace Eclipse
{
	static void Play(Entity entity)
	{
		AudioComponent& audioComp = engine->world.GetComponent<AudioComponent>(entity);
		TransformComponent& transComp = engine->world.GetComponent<TransformComponent>(entity);
		engine->audioManager.Play3DSounds(audioComp, transComp);
	}
}