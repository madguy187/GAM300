#pragma once

#include "GLEW/include/GL/glew.h"
#include "GLM/glm/glm.hpp"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"
#include "Graphics/Animation/AnimationManager.h"

namespace Eclipse
{
	struct AnimationComponent
	{
		//META_DATA(AnimationComponent);

		std::vector<glm::mat4> m_Transforms;
		Animation m_CurrentAnimation;
		float m_CurrentTime = 0.0f;
		float m_DeltaTime = 0.0f;
		float modelLargestAxis = 0.0f;
	};
}