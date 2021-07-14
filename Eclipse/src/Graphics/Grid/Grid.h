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
		float Transparency = 1.0f;

	public:
		ECVec3 GridColour{ 0.0f,0.0f,0.0f };
		float GridScale = 0.05f;
		bool Visible = true;

		Quad* GetModelReference();
		Shader* GetShaderReference();
		float GetTransparency();
		unsigned int GetGridID();
		void SetTransparency(float in);
		void UseFrameBuffer(unsigned int FramebufferID);
		void CheckUniformLocation(unsigned int GridID);
		float GetGridScale();
		void SetGridScale(float in);
		bool CheckShowGrid();
		void SetGridToShow(bool in);

		void Init();
		void DrawGrid();


		~Grid();


		// Useless Stuffs for Now
		//IModel* modelRef;
	};
}