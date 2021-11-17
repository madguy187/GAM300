#include "pch.h"
#include "AnimationFSM.h"
#include "ECS/SystemManager/Systems/System/EntityCompSystem/EntityCompSystem.h"

namespace Eclipse
{
	AnimationFSM::AnimationFSM() :
		FSM<AnimationState>{}, m_ID{ MAX_ENTITY }
	{}

	AnimationFSM::AnimationFSM(Entity ID) :
		FSM<AnimationState>{}, m_ID{ ID }
	{
		auto& meshCom = engine->world.GetComponent<MeshComponent>(m_ID);
		std::string meshName = std::string(meshCom.MeshName.data());

		switch (str2int(meshName.c_str()))
		{
		case str2int("Frog"):
			States.push_back(AnimationState::IDLE);
			States.push_back(AnimationState::MOTION);
			States.push_back(AnimationState::RUN);
			break;
		case str2int("MutantMesh"):
			States.push_back(AnimationState::DANCE);
			break;
		case str2int("Body"):
			States.push_back(AnimationState::WALK);
			States.push_back(AnimationState::DEATH);
			break;
		default:
			break;
		}
	}

	void AnimationFSM::UpdateState(AnimationState state)
	{
		(void)state;

		engine->gAnimationManager.UpdateAnimation(m_ID, engine->Game_Clock.get_fixedDeltaTime());

		auto& meshCom = engine->world.GetComponent<MeshComponent>(m_ID);
		std::string meshName = std::string(meshCom.MeshName.data());
		
		if (GetTimeInCurrentState() > 5.0f)
		{
			if (StateIndex < States.size())
				SetNextState(States[StateIndex++]);
			else
				StateIndex = 0;
		}
	}

	void AnimationFSM::BeginState(AnimationState state)
	{
		engine->gAnimationManager.ChangeAnimationState(m_ID, state);
	}

	void AnimationFSM::EndState(AnimationState state)
	{
		if (GetNextState() == AnimationState::DEATH)
		{
			auto entcomSys = engine->world.GetSystem<EntityCompSystem>();
			auto& meshCom = engine->world.GetComponent<MeshComponent>(m_ID);
			std::string meshName = std::string(meshCom.MeshName.data());
			DestroyEvent de;
			Entity toDie = MAX_ENTITY;

			switch (str2int(meshName.c_str()))
			{
			case str2int("Body"):
				de.ID = m_ID;
				de.IsDead = true;
				EventSystem<DestroyEvent>::dispatchEvent(de);

				// temp
				toDie = entcomSys->FindEntity("AnimationTriggerBox");

				if (toDie != MAX_ENTITY)
				{
					de.ID = toDie;
					de.IsDead = true;
					EventSystem<DestroyEvent>::dispatchEvent(de);
				}

				m_ID = MAX_ENTITY;
				break;
			default:
				break;
			}
		}

		(void)state;
	}

	void AnimationFSM::SetEntity(Entity ID)
	{
		m_ID = ID;
	}
}
