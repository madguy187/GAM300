#pragma once
#include "Graphics/ModelFactory/BasicPrimitives/Quad.h"

namespace Eclipse
{
	class Grid
	{
	private:
		unsigned int GridID;
		Quad* WholeGrid = nullptr;
		Shader* ShaderRef = nullptr;
		ECVec3 GridColour{ 1.0f,1.0f,1.0f };
		float Transparency = 1.0f;
		float GridScale = 0.05f;

	public:

		Quad* GetModelReference();
		Shader* GetShaderReference();
		float GetTransparency();
		unsigned int GetGridID();
		void SetTransparency(float in);
		void UseFrameBuffer(unsigned int FramebufferID);
		void CheckUniformLocation(unsigned int GridID);
		float GetGridScale();
		void SetGridScale(float in);

		void Init();
		void DrawGrid();


		~Grid();


		// Useless Stuffs for Now
		//IModel* modelRef;
	};
}