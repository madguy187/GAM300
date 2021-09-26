#pragma once
#include "ECS/ComponentManager/Components/PointLightComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
	typedef std::unordered_map<unsigned int, PointLightComponent*> PointLightContainer;
	using PointL = std::unordered_map<unsigned int, PointLightComponent*>::iterator;

	class PointLight
	{
	private:
		// Global Counter for all pointlights to loop in shader
		inline static unsigned int PointLightCounter = 0;

	public:
		// Returns the number of Pointlights
		unsigned int GetNumberOfPointLights();
		// Create PointLight
		static void CreatePointLight(unsigned int CreatedID);
		// PointLight
		void Draw(unsigned int EntityId ,PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);

	private:
		// Update Shader Variables
		void CheckUniformLoc(Shader* _shdrpgm, PointLightComponent& hi, int index, unsigned int containersize);

	private:
		// Container for all Pointlights
		inline static PointLightContainer _pointlights;
		// Returns the PointLight Container
		PointLightContainer GetContainer();
		// Remove PointLight From Container
		static bool DeletePointLight(unsigned int EntityID);
		// Pointlight Old Draw
		void DrawPointLights(unsigned int framebufferID);
		// Insert pointlight
		bool InsertPointLight(PointLightComponent& in);
		// Clear
		void ClearContainer();
	};
}
