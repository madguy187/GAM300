#pragma once
#include <map>
#include "ECS/ComponentManager/Components/DirectionalLightComponent.h"
#include "ECS/ComponentManager/Components/LightComponent.h"

namespace Eclipse
{
	typedef std::unordered_map<int, DirectionalLightComponent*> DirectionalLightContainer;
	using DLIT = std::unordered_map<int, DirectionalLightComponent*>::iterator;

	class DirectionalLight
	{
	private:
		// Global Counter for all DirectionalLight to loop in shader
		inline static unsigned int DirectionalLightcounter = 0;

	public:
		// Create Directional Light
		static void CreateDirectionalLight(unsigned int CreatedID);
		// Directional Light Draw
		void Draw(unsigned int EntityId,DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);
		// First Created Light
		void FirstGlobalLight();

	private:
		// Update Shader Variables
		void CheckUniformLoc(Shader* _shdrpgm, DirectionalLightComponent& hi, int index, unsigned int containersize);

	private:
		// Directional Light Container
		inline static Eclipse::DirectionalLightContainer _DirectionalLight;
		// Returns DirectionalLight Container
		DirectionalLightContainer GetContainer();
		// insert DirectionalLight
		bool InsertDirectionalLight(DirectionalLightComponent& in);
		// Clear 
		void ClearContainer();
		// Delete Directional Light
		static bool DeleteDirectionalLight(unsigned int EntityID);

	};
}
