#include "pch.h"
#include "FSMManager.h"
#include "../StateDesigns/Animation/AnimationFSM.h"

namespace Eclipse
{
	void FSMManager::Init()
	{
		AddFSM<AnimationFSM>();
	}
}
