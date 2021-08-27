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

    void Grid::SetTransparency(float in)
    {
        Transparency = in;
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

    float Grid::GetTransparency()
    {
        return Transparency;
    }

    void Grid::Init()
    {
        // Add Grid as an entity
        /*
        GridID = engine->world.CreateEntity();
        engine->world.AddComponent(GridID, EntityComponent{ EntityType::ENT_UNASSIGNED, "Grid", true });
        engine->world.AddComponent(GridID, TransformComponent{});

        engine->editorManager->EntityHierarchyList_.push_back(GridID);
        engine->editorManager->EntityToTypeMap_.insert(std::pair<Entity, EntityType>(GridID, EntityType::ENT_UNASSIGNED));
        */

        WholeGrid = new Quad;
        ShaderRef = &(Graphics::shaderpgms.find("Grid")->second);
        ShaderName = Graphics::shaderpgms.find("Grid")->first;

        CalculateGridSettings();

        if (WholeGrid != nullptr)
        {
            EDITOR_LOG_INFO("Grid Created");
        }

        CalculateGridCoordinates();
    }

    void Grid::CalculateGridCoordinates()
    {
        // Length of Grid on each side.
        ECVec3 TempLength = XYZ_Length;

        // Only max have this number of tiles
        GridArray.reserve(TotalTiles);

        // Initialise everything 
        Tile NewTile(GridScale, true);
        NewTile.CenterPoint = StartingPosition;
        GridArray.push_back(NewTile);

        for (int i = 0; i < (TotalTiles - 1); i++)
        {
            Tile NewTile(GridScale, false);
            GridArray.push_back(NewTile);
        }

        std::cout << "Grid Size " << GridArray.size() << std::endl;

        for (int y = 0; y < GridSize; y++)
        {
            for (int z = 0; z < GridSize; z++)
            {
                for (int x = 0; x < GridSize; x++)
                {
                    unsigned int PreviousIndex = x;
                    --PreviousIndex;

                    if (GridArray[x].FirstTile == true)
                        continue;

                    GridArray[(z * 3) + x + y].CenterPoint.setX(GridArray[PreviousIndex].CenterPoint.getX() + GridScale);
                    GridArray[(z * 3) + x + y].CenterPoint.setY(GridArray[PreviousIndex].CenterPoint.getY());
                    GridArray[(z * 3) + x + y].CenterPoint.setZ(GridArray[PreviousIndex].CenterPoint.getZ());
                }
            }
        }
    }

    void Grid::CalculateGridSettings()
    {

        ECVec3 CurrentGridPosition = { 0,0,0 };
        auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();

        float SnapValue = scene->GetSnapSettings().mPosSnapValue;
        GridScale = (SnapValue);

        TotalTiles = GridSize * GridSize * GridSize;

        Length = GridSize * GridScale;
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
    }

    void Grid::CalculateStartingPoint(ECVec3& MinimumIn, ECVec3& Maximum)
    {
        StartingPosition.setX((MinimumIn.getX() + (MinimumIn.getX() + GridScale)) / 2);
        StartingPosition.setY((MinimumIn.getY() + (MinimumIn.getY() + GridScale)) / 2);
        StartingPosition.setZ((Maximum.getZ() + (Maximum.getZ() - GridScale)) / 2);
    }

    void Grid::DrawGrid(unsigned int FrameBufferID)
    {
        UseFrameBuffer(FrameBufferID);

        ShaderRef->Use();

        // Part 2: Bind the object's VAO handle using glBindVertexArray
        glBindVertexArray(WholeGrid->GetVaoID());

        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Check here
        CheckUniformLocation(GridID);

        if (Visible == true)
        {
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
}