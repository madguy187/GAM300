#include "pch.h"
#include "Grid.h"
#include "Editor/Windows/Scene/SceneView.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"

namespace Eclipse
{
    unsigned int Grid::GetGridID()
    {
        return GridID;
    }

    void Grid::UseFrameBuffer(unsigned int FramebufferID)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID);
    }

    void Grid::CheckUniformLocation(unsigned int GridID)
    {
        // Be in line with Editor Settings
        auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();
        float SnapValue = scene->GetSnapSettings().mPosSnapValue;
        GridScale = (SnapValue);

        CameraComponent camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
        //TransformComponent& trans = engine->world.GetComponent<TransformComponent>(GridID);

        float nearplane = camera.nearPlane;
        float farplane = camera.farPlane;

        GLint uniform_var_loc1 = ShaderRef->GetLocation("QuadScale");
        GLint uniform_var_loc2 = ShaderRef->GetLocation("viewMtx");
        GLint uniform_var_loc3 = ShaderRef->GetLocation("projMtx");
        GLint uniform_var_loc4 = ShaderRef->GetLocation("GridColour");
        GLint uniform_var_loc5 = ShaderRef->GetLocation("CameraNearPlane");
        GLint uniform_var_loc6 = ShaderRef->GetLocation("CameraFarPlane");
        GLint uniform_var_loc7 = ShaderRef->GetLocation("Inner");
        GLint uniform_var_loc8 = ShaderRef->GetLocation("CheckDraw");
        GLint uniform_var_loc9 = ShaderRef->GetLocation("Z_Thickness");
        GLint uniform_var_loc10 = ShaderRef->GetLocation("X_Thickness");
        GLint uniform_var_loc11 = ShaderRef->GetLocation("XAxis_Colour");
        GLint uniform_var_loc12 = ShaderRef->GetLocation("ZAxis_Colour");

        GLCall(glUniform1i(uniform_var_loc1, GridScale));
        glUniformMatrix4fv(uniform_var_loc2, 1, GL_FALSE, glm::value_ptr(camera.viewMtx));
        glUniformMatrix4fv(uniform_var_loc3, 1, GL_FALSE, glm::value_ptr(camera.projMtx));
        GLCall(glUniform3f(uniform_var_loc4, GridColour.getX(), GridColour.getY(), GridColour.getZ()));
        GLCall(glUniform1f(uniform_var_loc5, nearplane));
        GLCall(glUniform1f(uniform_var_loc6, farplane));
        GLCall(glUniform1i(uniform_var_loc7, InnerRatio));
        GLCall(glUniform1i(uniform_var_loc8, Visible));
        GLCall(glUniform1i(uniform_var_loc9, SingleZAxisLineThickness));
        GLCall(glUniform1i(uniform_var_loc10, SingleXAxisLineThickness));
        GLCall(glUniform1f(uniform_var_loc11, XAxisColour));
        GLCall(glUniform1f(uniform_var_loc12, ZAxisColour));
    }

    float Grid::GetGridScale()
    {
        return GridScale;
    }

    void Grid::SetGridScale(float in)
    {
        GridScale = in;
    }

    bool Grid::CheckShowGrid()
    {
        return Visible;
    }

    void Grid::SetGridToShow(bool in)
    {
        Visible = in;
    }

    void Grid::DebugPrint()
    {
        std::cout << "Grid Debug Print" << std::endl;
        std::cout << "===========================" << std::endl;
        std::cout << "Grid ID : " << GridID << std::endl;
        std::cout << "Per Square Scale : " << GridScale << std::endl;
        std::cout << "Inner Ratio : " << InnerRatio << std::endl;
        std::cout << "How Many Squares inside each Grid : " << GridScale / InnerRatio << std::endl;
        std::cout << "Grid Colour : " << GridColour.getX() << " " << GridColour.getY() << " " << GridColour.getZ() << std::endl;
        std::cout << "Visible ? : " << Visible << std::endl;
        std::cout << "===========================" << std::endl;
    }

    ECVec3 Grid::GetGridColour()
    {
        return GridColour;
    }

    void Grid::SetGridColour(ECVec3& in)
    {
        GridColour.setX(in.getX());
        GridColour.setY(in.getY());
        GridColour.setZ(in.getZ());
    }

    int Grid::GetInnerRatio()
    {
        return InnerRatio;
    }

    void Grid::SetInnerRatio(int& in)
    {
        InnerRatio = in;
    }

    int Grid::GetSingleZ_Thickness()
    {
        return SingleZAxisLineThickness;
    }

    void Grid::SetSingleZ_Thickness(int& in)
    {
        SingleZAxisLineThickness = in;
    }

    int Grid::GetSingleX_Thickness()
    {
        return SingleXAxisLineThickness;
    }

    float Grid::GetXAxisColour()
    {
        return XAxisColour;
    }

    float Grid::GetZAxisColour()
    {
        return ZAxisColour;
    }

    void Grid::SetSingleX_Thickness(int& in)
    {
        SingleXAxisLineThickness = in;
    }

    void Grid::SetXAxisColour(float& in)
    {
        XAxisColour = in;
    }

    void Grid::SetZAxisColour(float& in)
    {
        ZAxisColour = in;
    }

    Quad* Grid::GetModelReference()
    {
        return WholeGrid;
    }

    Shader* Grid::GetShaderReference()
    {
        return ShaderRef;
    }

    void Grid::Init()
    {
        InsertAsDebugBox();

        WholeGrid = new Quad;
        ShaderRef = &(Graphics::shaderpgms["Grid"]);

        if (WholeGrid != nullptr && CalculateGridSettings() && CalculateGridCoordinates())
        {
            EDITOR_LOG_INFO("Grid Created");
        }
    }

    bool Grid::CalculateGridCoordinates()
    {
        // Length of Grid on each side.
        ECVec3 TempLength = XYZ_Length;

        // Only max have this number of tiles
        GridArray.reserve(TotalTiles);

        for (int y = 0; y < TotalTiles; y += (GridSize * GridSize))
        {
            for (int z = 0; z < GridSize; z++)
            {
                for (int x = 0; x < GridSize; x++)
                {
                    unsigned int Index = (z * GridSize) + x + y;
                    float HalfExtent = (GridScale / 2);

                    Tile NewTile(GridScale, true);
                    GridArray.push_back(NewTile);

                    // Set Center Point
                    glm::vec3 Center = { StartingPosition.getX() + (x * GridScale) , StartingPosition.getY() + ((y / (GridSize * GridSize)) * GridScale) , StartingPosition.getZ() - (z * GridScale) };
                    GridArray[Index].CenterPoint = ECVec3{ Center.x, Center.y, Center.z };

                    // Set Maximum Point
                    glm::vec3 Maximum = { Center.x + HalfExtent , Center.y + HalfExtent , Center.z + HalfExtent };
                    GridArray[Index].MaximumPoint = ECVec3{ Maximum.x, Maximum.y, Maximum.z };

                    // Set Minimum Point
                    glm::vec3 Minimum = { Center.x - HalfExtent , Center.y - HalfExtent , Center.z - HalfExtent };
                    GridArray[Index].MinimumPoint = ECVec3{ Minimum.x, Minimum.y, Minimum.z };

                    // Set Width
                    GridArray[Index].Width = Maximum.x - Minimum.x;

                    if (AddDebugBoxes)
                    {
                        BoundingRegion br(GridArray[Index].CenterPoint.ConvertToGlmVec3Type(), { GridScale ,GridScale ,GridScale });
                        engine->GraphicsManager.AllAABBs.AddInstance(br);
                    }

                    // Set AABB For Dynamic Tree
                    GridArray[Index].aabb.SetMaxMin(GridArray[Index].MaximumPoint, GridArray[Index].MinimumPoint, Index);
                    std::shared_ptr<Tile>first = std::make_shared<Tile>(GridArray[Index]);
                    engine->CollisionGridTree.InsertObject(first);

                    // Insert into container
                    gridArray.insert({ Index ,GridArray[Index] });
                }
            }
        }

        DebugPrintCoorindates(GridArray);

        return true;
    }

    bool Grid::CalculateGridSettings()
    {

        ECVec3 CurrentGridPosition = { 0,0,0 };
        auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();

        float SnapValue = scene->GetSnapSettings().mPosSnapValue;
        GridScale = (SnapValue);

        TotalTiles = GridSize * GridSize * GridSize;

        float Length = GridSize * GridScale;
        XYZ_Length.setX(Length);
        XYZ_Length.setY(Length);
        XYZ_Length.setZ(Length);

        ECVec3 Half;
        Half.setX(XYZ_Length.getX() / 2.0f);
        Half.setY(XYZ_Length.getY() / 2.0f);
        Half.setZ(XYZ_Length.getZ() / 2.0f);

        Minimum.setX(CurrentGridPosition.getX() - Half.getX());
        Minimum.setY(CurrentGridPosition.getY());
        Minimum.setZ(CurrentGridPosition.getZ() - Half.getZ());

        Maximum.setX(CurrentGridPosition.getX() + Half.getX());
        Maximum.setY(CurrentGridPosition.getY() + XYZ_Length.getY());
        Maximum.setZ(CurrentGridPosition.getZ() + Half.getZ());

        CalculateStartingPoint(Minimum, Maximum);

        return true;
    }

    void Grid::CalculateStartingPoint(ECVec3& MinimumIn, ECVec3& Maximum)
    {
        StartingPosition.setX((MinimumIn.getX() + (MinimumIn.getX() + GridScale)) / 2);
        StartingPosition.setY((MinimumIn.getY() + (MinimumIn.getY() + GridScale)) / 2);
        StartingPosition.setZ((Maximum.getZ() + (Maximum.getZ() - GridScale)) / 2);
    }

    void Grid::DebugPrintCoorindates(std::vector<Tile>& in)
    {
        if (PrintDebug == false)
            return;

        for (int y = 0; y < TotalTiles; y += (GridSize * GridSize))
        {
            for (int z = 0; z < GridSize; z++)
            {
                for (int x = 0; x < GridSize; x++)
                {
                    unsigned int Index = (z * GridSize) + x + y;

                    std::cout << "=================================" << std::endl;
                    std::cout << "= ID            :" << Index << std::endl;
                    std::cout << "= Maximum Point :" << gridArray[Index].MaximumPoint << std::endl;
                    std::cout << "= Minimum Point :" << gridArray[Index].MinimumPoint << std::endl;
                    std::cout << "= Center Point  :" << gridArray[Index].CenterPoint << std::endl;
                    std::cout << "= Width         :" << gridArray[Index].Width << std::endl;
                    std::cout << "=================================" << std::endl;
                }
            }
        }
    }

    void Grid::InsertAsDebugBox()
    {
        BoundingRegion br({ 0,0,0 }, { GridScale * GridSize , 0  ,GridScale * GridSize });
        engine->GraphicsManager.AllAABBs.AddInstance(br);
    }

    bool Grid::CheckTileOccupied(TILE_ID& tileID)
    {
        if (gridArray[tileID].Occupied)
        {
            return true;
        }

        return false;
    }

    std::vector<TILE_ID> Grid::GetOccupiedTiles(std::vector<TILE_ID>& in)
    {
        std::vector<TILE_ID> Temp;

        for (int Index = 0; Index < in.size(); Index++)
        {
            if (gridArray[in[Index]].Occupied == true)
            {
                Temp.push_back(gridArray[in[Index]].aabb.GetEntityID());
            }
        }

        return Temp;
    }

    ECVec3 Grid::SnapCalculate(ECVec3& p, float s)
    {
        float snapX = p.getX() + ((std::floor(p.getX() / s) - p.getX() / 5) * 5);
        float snapY = p.getY() + ((std::floor(p.getY() / s) - p.getY() / 5) * 5);
        float snapZ = p.getZ() + ((std::floor(p.getZ() / s) - p.getZ() / 5) * 5);

        return ECVec3(snapX, snapY, snapZ);
    }

    void Grid::DrawGrid(unsigned int FrameBufferID)
    {
        UseFrameBuffer(FrameBufferID);

        ShaderRef->Use();

        if (Visible == true)
        {
            // Part 2: Bind the object's VAO handle using glBindVertexArray
            glBindVertexArray(WholeGrid->GetVaoID());

            glEnable(GL_DEPTH_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            // Check here
            CheckUniformLocation(GridID);

            // Draw
            glDrawElements(WholeGrid->GetPrimitiveType(), WholeGrid->GetDrawCount(), GL_UNSIGNED_SHORT, NULL);
        }

        // Part 5: Clean up
        glBindVertexArray(0);
        ShaderRef->UnUse();
    }

    Grid::~Grid()
    {
        // Handle Memory for WholeGrid*
        if (WholeGrid != nullptr)
        {
            delete WholeGrid;
        }
    }

    float Grid::GetDistanceToObject(unsigned int indexIn)
    {
        return gridArray[indexIn].aabb.DistanceToObject;;
    }

    void Grid::SetDistance(AABBNode& Nodein, AABBComponent& aabbin, unsigned int id)
    {
        Nodein.aabb.DistanceToObject = Nodein.aabb.Max.x - aabbin.max.getX();
        /*engine->GridManager->*/gridArray[Nodein.aabb.GetEntityID()].aabb.DistanceToObject = Nodein.aabb.DistanceToObject;
    }

    void Grid::SetDistance(AABBNode& Nodein, AABBComponent& aabbin)
    {
        Nodein.aabb.DistanceToObject = Nodein.aabb.Max.x - aabbin.max.getX();
    }

    void Grid::SetDistance(AABBNode& Nodein, DYN_AABB& aabbin )
    {
        Nodein.aabb.DistanceToObject = Nodein.aabb.Max.x - aabbin.Max.x;
        engine->GridManager->gridArray[Nodein.aabb.GetEntityID()].aabb.DistanceToObject = Nodein.aabb.DistanceToObject;
    }

    void Grid::SetPosition(TransformComponent& in, unsigned int id)
    {
        in.position = gridArray[id].CenterPoint;
    }

    bool Grid::GetGridSystemIsRunning()
    {
        return GridSystemIsRunning;
    }

    void Grid::SetGridSystemIsRunning(bool in)
    {
        GridSystemIsRunning = in;
    }

    DYN_AABB Tile::getAABB() const
    {
        return aabb;
    }
}