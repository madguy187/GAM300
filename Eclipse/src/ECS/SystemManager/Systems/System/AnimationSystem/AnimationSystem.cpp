#include "pch.h"
#include "AnimationSystem.h"

bool once = false;

void Eclipse::AnimationSystem::Init()
{
}

void Eclipse::AnimationSystem::Update()
{
	//if (!once)
	//{
	//	std::cout << "Size (Before): " << engine->gAnimationManager.GetAnimationMap().size() << std::endl;
	//
	//	engine->gAssimpAnimator.LoadAnimationFile("..//Eclipse//src//Assets//Models");
	//	engine->gAnimationManager.PopulateAnimationMapFromVector(engine->gAssimpAnimator.AnimationContainer);
	//
	//	std::cout << "Size (After): " << engine->gAnimationManager.GetAnimationMap().size() << std::endl;
	//
	//	once = true;
	//}

	for (auto& it : mEntities)
	{
		engine->gAnimationManager.UpdateAnimation(it, engine->Game_Clock.get_DeltaTime());
	}
}
