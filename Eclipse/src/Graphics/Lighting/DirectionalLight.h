#pragma once
#include <map>
#include "ECS/ComponentManager/Components/DirectionalLightComponent.h"

namespace Eclipse
{
	typedef std::unordered_map<int, DirectionalLightComponent*> DirectionalLightContainer;
	using DLIT = std::unordered_map<int, DirectionalLightComponent*>::iterator;

	class DirectionalLight
	{
	private:
		// Directional Light Container
		inline static Eclipse::DirectionalLightContainer _DirectionalLight;
		// Global Counter for all DirectionalLight to loop in shader
		inline static unsigned int counter = 0;

	public:
		// Returns DirectionalLight Container
		DirectionalLightContainer GetContainer();
		// Create Directional Light
		static void CreateDirectionalLight(unsigned int CreatedID);
		// Delete Directional Light
		static bool DeleteDirectionalLight(unsigned int EntityID);
		// Directional Light Draw
		void Draw(DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);
		// First Created Light
		void FirstGlobalLight();
		// insert DirectionalLight
		bool InsertDirectionalLight(DirectionalLightComponent& in);
		// Clear 
		void ClearContainer();

	private:
		// Update Shader Variables
		void CheckUniformLoc(Shader* _shdrpgm, DirectionalLightComponent& hi, int index, unsigned int containersize);
	};
}
