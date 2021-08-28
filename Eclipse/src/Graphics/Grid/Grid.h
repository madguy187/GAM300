#pragma once
#include "Graphics/ModelFactory/BasicPrimitives/Quad.h"

namespace Eclipse
{
	// GridScale % InnerRatio Should be 0 as it gives u the number of boxes inside a grid

	struct Tile
	{
		Tile() {}
		Tile(float UnitLengthofeachtile , bool in) : 
			width(UnitLengthofeachtile),
			FirstTile(in)
		{

		}

		ECVec3 MinimumPoint{ 0.0f,0.0f,0.0f };
		ECVec3 MaximumPoint{ 0.0f,0.0f,0.0f };
		ECVec3 CenterPoint{ 0.0f,0.0f,0.0f };
		bool FirstTile = false;
		float width; //width = height!
	};

	class Grid
	{
	private:
		unsigned int GridID;
		float GridScale = 5.0f; // Unit length of each grid
		int InnerRatio = 1;
		int SingleXAxisLineThickness = 5;
		int SingleZAxisLineThickness = 5;
		float XAxisColour = 1.0f;
		float ZAxisColour = 1.0f;
		ECVec3 GridColour{ 0.3f,0.3f,0.3f };
		Quad* WholeGrid = nullptr;
		Shader* ShaderRef = nullptr;
		std::string ShaderName;

		// Coordinates Setup
		unsigned int GridSize = 3; // Number of tiles each side
		unsigned int TotalTiles = 0;
		float Grid3x3[10][10][10];
		ECVec3 XYZ_Length{ 0, 0, 0 };
		ECVec3 Minimum{ 0, 0, 0 };
		ECVec3 Maximum{ 0, 0, 0 };
		std::vector<Tile> GridArray; //array of all the points;
		std::map<unsigned int, Tile> gridArray; //key = the grid count;
		unsigned int Length = 0;
		ECVec3 StartingPosition{ 0.0f,0.0f,0.0f }; // Starting position of the bottom left most tile

		// Not used yet
		float Fading = 0.0f;
		float Transparency = 1.0f;

	public:
		bool Visible = true;

		Quad* GetModelReference();
		Shader* GetShaderReference();
		ECVec3 GetGridColour();
		float GetTransparency();
		float GetGridScale();
		unsigned int GetGridID();
		int GetInnerRatio();
		int GetSingleZ_Thickness();
		int GetSingleX_Thickness();
		float GetXAxisColour();
		float GetZAxisColour();

		void SetTransparency(float in);
		void SetGridToShow(bool in);
		void SetGridScale(float in);
		void SetInnerRatio(int& in);
		void SetGridColour(ECVec3& in);
		void SetSingleZ_Thickness(int& in);
		void SetSingleX_Thickness(int& in);
		void SetXAxisColour(float& in);
		void SetZAxisColour(float& in);

		void DebugPrint();
		bool CheckShowGrid();
		void UseFrameBuffer(unsigned int FramebufferID);
		void CheckUniformLocation(unsigned int GridID);
		void Init();
		void CalculateGridCoordinates();
		void CalculateGridSettings();
		void CalculateStartingPoint(ECVec3& MinimumIn, ECVec3& Maximum);
		void DebugPrintCoorindates(std::vector<Tile>& in);

		// Drawing of Grid , Must render last
		void DrawGrid(unsigned int FrameBufferID);

		~Grid();
	};
}