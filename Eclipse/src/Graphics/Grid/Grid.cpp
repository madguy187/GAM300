#include "pch.h"
#include "Grid.h"
#include "Editor/Windows/Scene/Scene.h"
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
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(GridID);

		float nearplane = camera.nearPlane; // 0.1; // camera.nearPlane;
		float farplane = camera.farPlane;   // 100.0f; // camera.farPlane;

		//GLint uniform_var_loc1 = ShaderRef->GetLocation("QuadScale");
		GLint uniform_var_loc2 = ShaderRef->GetLocation("viewMtx");
		GLint uniform_var_loc3 = ShaderRef->GetLocation("projMtx");
		GLint uniform_var_loc4 = ShaderRef->GetLocation("GridColour");
		//GLint uniform_var_loc5 = ShaderRef->GetLocation("near");
		//GLint uniform_var_loc6 = ShaderRef->GetLocation("far");
		//GLint uniform_var_loc7 = ShaderRef->GetLocation("InsideQuadCount");
		//GLint uniform_var_loc8 = ShaderRef->GetLocation("CheckDraw");

		//GLCall(glUniform1f(uniform_var_loc1, GridScale));
		//GLCall(glUniform1f(uniform_var_loc7, InsideQuadCount));
		//GLCall(glUniform1f(uniform_var_loc5, nearplane));
		//GLCall(glUniform1f(uniform_var_loc6, farplane));
		glUniformMatrix4fv(uniform_var_loc2, 1, GL_FALSE, glm::value_ptr(camera.viewMtx));
		glUniformMatrix4fv(uniform_var_loc3, 1, GL_FALSE, glm::value_ptr(camera.projMtx));
		GLCall(glUniform3f(uniform_var_loc4, GridColour.getX(), GridColour.getY(), GridColour.getZ()));
		//GLCall(glUniform1i(uniform_var_loc8, Visible));
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
		std::cout << "Grid Scale : " << GridScale << std::endl;
		std::cout << "Inside Quad Count : " << InsideQuadCount << std::endl;
		std::cout << "Grid Colour : " << GridColour.getX() << " " << GridColour.getY() << " " << GridColour.getZ() << std::endl;
		std::cout << "Visible ? : " << Visible << std::endl;
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
		return 0.0f;
	}

	void Grid::Init()
	{
		GridID = engine->world.CreateEntity();
		engine->world.AddComponent(GridID, EntityComponent{ EntityType::ENT_UNASSIGNED, "Grid", true });
		engine->world.AddComponent(GridID, TransformComponent{});

		engine->editorManager->EntityHierarchyList_.push_back(GridID);
		engine->editorManager->EntityToTypeMap_.insert(std::pair<Entity, EntityType>(GridID, EntityType::ENT_UNASSIGNED));


		WholeGrid = new Quad;
		ShaderRef = &(Graphics::shaderpgms.find("Grid")->second);

		std::cout << WholeGrid->GetDrawCount() << std::endl;

		// Useless Stuffs for now
		//modelRef = Graphics::models.find("square")->second.get();
	}

	void Grid::DrawGrid()
	{
		UseFrameBuffer(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());

		ShaderRef->Use();

		// Part 2: Bind the object's VAO handle using glBindVertexArray
		glBindVertexArray(WholeGrid->GetVaoID());

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDisable(GL_CULL_FACE);
		glEnable(GL_LINE_SMOOTH);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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