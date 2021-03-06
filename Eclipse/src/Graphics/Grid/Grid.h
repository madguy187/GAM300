#ifndef GRID_H
#define GRID_H
#include "Graphics/ModelFactory/BasicPrimitives/Quad.h"
#include "Graphics/Grid/IAABB.h"
#include "Graphics/Grid/AABB.h"
#include "Graphics/Instancing/DebugBoxes/Box.h"
#include "ECS/ComponentManager/Components/AABBComponent.h"
#include "Graphics/Grid/AABBTree.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
    // GridScale % InnerRatio Should be 0 as it gives u the number of boxes inside a grid

    // Tile ID
    typedef unsigned int TILE_ID;

    struct Tile : public IAABB
    {
        Tile() { }

        Tile(float UnitLengthofeachtile, bool in) :
            Width(UnitLengthofeachtile),
            FirstTile(in)
        {
            // If its the First Tile , Will be 0
            if (in == true)
            {
                aabb.SetEntityID(0);
            }

            aabb.SetIsGrid(true);
        }

        // Tile ID
        TILE_ID GridID = 0;
        // Tile AABB to be used in DynamicAABBTree
        DYN_AABB aabb;
        // Tile Minimum Point
        ECVec3 MinimumPoint{ 0.0f,0.0f,0.0f };
        // Tile Maximum Point
        ECVec3 MaximumPoint{ 0.0f,0.0f,0.0f };
        // Tile Center Position / Position
        ECVec3 CenterPoint{ 0.0f,0.0f,0.0f };
        // Tile Check isit First Tile
        bool FirstTile = false;
        // Tile Current Height/Width
        float Width = 0.0f;
        // Tile Occupied? 
        bool Occupied = false;
        // Get AABB For Tile
        DYN_AABB getAABB() const override;
    };

    class Grid
    {
    private:
        // The Whole Grid ID
        unsigned int GridID;
        // Unit length of each Title
        float GridScale = 5.0f;
        // GridScale % InnerRatio Should be 0 as it gives u the number of boxes inside a Tile
        int InnerRatio = 1;
        // X Grid Axis Outline Thickness
        int SingleXAxisLineThickness = 5;
        // Z Grid Axis Outline Thickness
        int SingleZAxisLineThickness = 5;
        // X Grid Axis Outline Colour
        float XAxisColour = 1.0f;
        // Z Grid Axis Outline Colour
        float ZAxisColour = 1.0f;
        // Whole Grid Colour
        ECVec3 GridColour{ 0.3f,0.3f,0.3f };
        // What Shape am i using to Render the Grid
        std::unique_ptr<Quad> WholeGrid = nullptr;
        // Shader Reference 
        Shader* ShaderRef = nullptr;

        // Number of Tiles Each Side.
        unsigned int GridSize = 16;
        // Total Number of Tiles for the Grid
        unsigned int TotalTiles = 0;
        // Flag to Add Debug Boxes for each Tile
        bool AddDebugBoxes = false;
        // Flag to Print out all the Coordinates of each tile
        bool PrintDebug = false;
        // X,Y,Z Grid Scale
        ECVec3 XYZ_Length{ 0.0f , 0.0f, 0.0f };
        // Grid Minimum Point
        ECVec3 Minimum{ 0.0f, 0, 0 };
        // Grid Maximum Point
        ECVec3 Maximum{ 0, 0, 0 };
        // Starting position of the Grid using bottom left most Tile
        ECVec3 StartingPosition{ 0.0f,0.0f,0.0f };
        // Vector of all the Tiles
        std::vector<std::shared_ptr<Tile>> GridArray;
        std::unordered_map<unsigned int, std::shared_ptr<Tile>> gridArray; //key = the grid count;
        bool GridSystemIsRunning = true;

    public:
        // Flag to Render Grid
        bool Visible = true;

        // Get Model Reference
        Shader* GetShaderReference();
        // Get Grid Colour
        ECVec3 GetGridColour();
        // Get Grid Scale (Uniform for X,Y,Z)
        float GetGridScale();
        // Get Grid ID
        unsigned int GetGridID();
        // Get Inner Ratio = GridScale % InnerRatio Should be 0 as it gives u the number of boxes inside a Tile
        int GetInnerRatio();
        // Get Z Axis Thickness
        int GetSingleZ_Thickness();
        // Get X Axis Thickness
        int GetSingleX_Thickness();
        // Get X Axis Colour
        float GetXAxisColour();
        // Get Z Axis Colour
        float GetZAxisColour();

        // Set Grid's Visible Flag to render
        void SetGridToShow(bool in);
        // Set Grid Scale
        void SetGridScale(float in);
        // Set Grid Inner Ratio
        void SetInnerRatio(int& in);
        // Set Grid Colour As a whole
        void SetGridColour(ECVec3& in);
        // Set Grid Z Axis Thickness
        void SetSingleZ_Thickness(int& in);
        // Set Grid X Axis Thickness
        void SetSingleX_Thickness(int& in);
        // Set Grid X Axis Colour
        void SetXAxisColour(float& in);
        // Set Grid X Axis Colour
        void SetZAxisColour(float& in);

        // Print Grid's Current Things
        void DebugPrint();
        // Check Grid's Visibility to Render
        bool CheckShowGrid();
        // Bind Current Framebuffer
        void UseFrameBuffer(unsigned int FramebufferID);
        // Update uniforms to Shader
        void CheckUniformLocation(unsigned int GridID);
        // Grid Init = Calculate Everything 
        void Init();
        // Calculate and insert Tiles for the Whole Grid
        bool CalculateGridCoordinates();
        // Calculate Grid Properties
        bool CalculateGridSettings();
        // Calculate Starting Point for the Grid which we going to use bottom left
        void CalculateStartingPoint(ECVec3& MinimumIn, ECVec3& Maximum);
        // Print All the Coordinates of each Tile in the Grid
        void DebugPrintCoorindates(std::vector<std::shared_ptr<Tile>>& in);
        // Insert Grid Outline
        void InsertAsDebugBox();
        // Check Current Tile Occupied or not
        bool CheckTileOccupied(TILE_ID& tileID);
        // Returns a vector of Tile IDs being Occupied
        std::vector<TILE_ID> GetOccupiedTiles(std::vector<TILE_ID>& in);
        // Drawing of Grid , Must render last
        void DrawGrid(FrameBufferMode in);
        // Destructor
        ~Grid();
        // Get Current Tile's distancetotheObject
        float GetDistanceToObject(unsigned int indexIn);
        // Set distance for Both the Tree and GridArray
        void SetDistance(AABBNode& Nodein, AABBComponent& aabbin, unsigned int id);
        // Set Distance For Current Node for Grid
        void SetDistance(AABBNode& Nodein, AABBComponent& aabbin);
        // Set Distance For Current Node for Grid
        void SetDistance(AABBNode& Nodein, DYN_AABB& aabbin);
        // Only check for 1 Intersection.
        void SetPosition(TransformComponent& in, unsigned int ID);

        bool GetGridSystemIsRunning();
        void SetGridSystemIsRunning(bool in);

        // UNSED ================================
        ECVec3 SnapCalculate(ECVec3& p, float s);
    };
}
#endif