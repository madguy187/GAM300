#include "pch.h"
#include "Graphics/RendererAPI/GraphicsManager.h"
#include "EntryPoint/EntryPoint.h"

namespace Eclipse
{
    void Eclipse::GraphicsManager::MassInit()
    {
        for (auto& i : GraphicThreads)
        {
            i.second->join();
        }
    }

    void Eclipse::GraphicsManager::RegisterThreads()
    {
        GraphicThreads.emplace("CreateFolders", std::make_unique<std::thread>(std::thread{ &GraphicsManager::CreateCompilerFolders }));
        GraphicThreads.emplace("Grid", std::make_unique<std::thread>(std::thread{ &GridSystem::Init }));
        GraphicThreads.emplace("Lighting", std::make_unique<std::thread>(std::thread{ &LightingSystem::Init }));
        GraphicThreads.emplace("FileWatch", std::make_unique<std::thread>(std::thread{ &FileWatchSystem::Init }));
    }

    void Eclipse::GraphicsManager::Pre_Render()
    {
        // Loading Configuration
        mRenderContext.init("../Dep/Configuration/configuration.json");

        // Loading Of Models , Shaders and etc.. 
        Graphics::load();

        // Clear the View
        mRenderContext.pre_render();

        // For grid
        GridQuad = std::make_unique<Quad>();
    }

    void Eclipse::GraphicsManager::Post_Render()
    {
        mRenderContext.post_render();
    }

    void Eclipse::GraphicsManager::End()
    {
        mRenderContext.end();
    }

    void Eclipse::GraphicsManager::Unload()
    {
        mRenderContext.end();
    }

    void Eclipse::GraphicsManager::CreatePrimitives(Entity ID, int ModelType)
    {
        switch (ModelType)
        {
        case 0:
        {
            engine->AssimpManager.CreateBasicPrimitives(ID, "Plane");

            //engine->world.AddComponent(ID, MaterialComponent{ MaterialModelType::MT_BASIC });
            //engine->world.AddComponent(ID, MeshComponent{});
            //MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
            //sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
            //sprite.modelRef = Graphics::models.find("Square")->first;
            //sprite.isQuad = true;

        }
        break;
        //case 1:
        //{
        //    engine->world.AddComponent(ID, MaterialComponent{ MaterialModelType::MT_BASIC });
        //    engine->world.AddComponent(ID, MeshComponent{});
        //    MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        //    sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        //    sprite.modelRef = Graphics::models.find("Circle")->first;
        //}
        //break;
        //case 2:
        //{
        //    engine->world.AddComponent(ID, MaterialComponent{ MaterialModelType::MT_BASIC });
        //    engine->world.AddComponent(ID, MeshComponent{});
        //    MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        //    sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        //    sprite.modelRef = Graphics::models.find("Triangle")->first;
        //}
        break;
        case 1:
        {
            engine->AssimpManager.CreateBasicPrimitives(ID, "Straight_Stairs");

            //engine->world.AddComponent(ID, MaterialComponent{ MaterialModelType::MT_BASIC });
            //engine->world.AddComponent(ID, MeshComponent{});
            //MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
            //sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
            //sprite.modelRef = Graphics::models.find("Lines")->first;
        }
        break;
        case 2:
        {
            engine->AssimpManager.CreateBasicPrimitives(ID, "Sphere");

            //engine->world.AddComponent(ID, MaterialComponent{ MaterialModelType::MT_BASIC });
            //engine->world.AddComponent(ID, MeshComponent{});
            //MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
            //sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
            //sprite.modelRef = Graphics::models.find("Sphere")->first;
            //MaterialComponent& Mat = engine->world.GetComponent<MaterialComponent>(ID);
            //Mat.hasTexture = true;
            //Mat.TextureRef = Graphics::textures.find("brick")->first;
        }
        break;
        case 3:
        {
            engine->AssimpManager.CreateBasicPrimitives(ID, "Cube");

            //engine->world.AddComponent(ID, MaterialComponent{ MaterialModelType::MT_BASIC });
            //engine->world.AddComponent(ID, MeshComponent{});
            //MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
            //sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
            //sprite.modelRef = Graphics::models.find("Cube")->first;
            //MaterialComponent& Mat = engine->world.GetComponent<MaterialComponent>(ID);
            //Mat.hasTexture = true;
            //Mat.TextureRef = Graphics::textures.find("FolderIcon")->first;
        }
        break;
        case 4:
        {
            engine->AssimpManager.CreateBasicPrimitives(ID, "Cylinder");

            //engine->world.AddComponent(ID, MaterialComponent{ MaterialModelType::MT_BASIC });
            //engine->world.AddComponent(ID, MeshComponent{});
            //MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
            //sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
            //sprite.modelRef = Graphics::models.find("Cylinder")->first;
        }
        break;
        case 5:
        {
            engine->AssimpManager.CreateBasicPrimitives(ID, "Cone");

            //engine->world.AddComponent(ID, MaterialComponent{ MaterialModelType::MT_BASIC });
            //engine->world.AddComponent(ID, MeshComponent{});
            //MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
            //sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
            //sprite.modelRef = Graphics::models.find("Cone")->first;
        }
        break;
        case 6:
        {
            engine->AssimpManager.CreateBasicPrimitives(ID, "Torus");

            //engine->world.AddComponent(ID, MaterialComponent{});
            //MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
            //mat.Modeltype = MaterialModelType::MT_BASIC;
            //engine->world.AddComponent(ID, MeshComponent{});
            //MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
            //sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
            //sprite.modelRef = Graphics::models.find("Torus")->first;
        }
        break;
        //case 9:
        //{
        //    engine->world.AddComponent(ID, MaterialComponent{});
        //    MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        //    mat.Modeltype = MaterialModelType::MT_BASIC;
        //    engine->world.AddComponent(ID, MeshComponent{});
        //    MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        //    sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        //    sprite.modelRef = Graphics::models.find("Pyramid")->first;

        //}
        //break;
        case 7:
        {
            engine->AssimpManager.CreateBasicPrimitives(ID, "ICOSphere");

            //engine->world.AddComponent(ID, MaterialComponent{});
            //MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
            //mat.Modeltype = MaterialModelType::MT_BASIC;
            //engine->world.AddComponent(ID, MeshComponent{});
            //MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
            //sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
            //sprite.modelRef = Graphics::models.find("Lines3D")->first;

        }
        break;
        case 8:
        {
            engine->AssimpManager.CreateBasicPrimitives(ID, "Plane");

            //engine->world.AddComponent(ID, MaterialComponent{});
            //MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
            //mat.Modeltype = MaterialModelType::MT_BASIC;
            //engine->world.AddComponent(ID, MeshComponent{});
            //MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
            //sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
            //sprite.modelRef = Graphics::models.find("Plane")->first;
        }
        break;
        case 9:
        {
            engine->AssimpManager.CreateBasicPrimitives(ID, "Curved_Stairs");
        }
        break;
        case 10:
        {
            engine->AssimpManager.CreateBasicPrimitives(ID, "Spiral_Stairs");
        }
        break;


        // pointlight
        case 11:
        {
            engine->LightManager.CreateLights(Eclipse::TypesOfLights::POINTLIGHT, ID);
        }
        break;
        // Directional
        case 12:
        {
            engine->LightManager.CreateLights(Eclipse::TypesOfLights::DIRECTIONAL, ID);
        }
        break;
        // SpotLight
        case 13:
        {
            engine->LightManager.CreateLights(Eclipse::TypesOfLights::SPOTLIGHT, ID);
        }
        break;
        }
    }

    void Eclipse::GraphicsManager::CreateSky(std::string _Dir)
    {
        if (SkyCount == 1)
        {
            ENGINE_LOG_ASSERT(false, "There is already 1 Cube Map Created");
            return;
        }

        Sky = std::make_unique<CubeMap>();
        Sky->CreateSky(_Dir);

        SkyCount++;
    }

    void Eclipse::GraphicsManager::LoadSky()
    {
        engine->GraphicsManager.CreateSky("src/Assets/Sky");
    }

    void Eclipse::GraphicsManager::RenderSky(FrameBufferMode mode)
    {
        if (DrawSky == true)
        {
            engine->gFrameBufferManager->UseFrameBuffer(mode);
            auto& shdrpgm = Graphics::shaderpgms["Sky"];

            Sky->Render(shdrpgm);
        }
    }

    void Eclipse::GraphicsManager::Draw(FrameBufferMode Mode, MeshComponent* _spritecomponent, GLenum mode, unsigned int ID, CameraComponent::CameraType _camType)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(_camType));
        engine->gFrameBufferManager->UseFrameBuffer(Mode);

        auto& shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];
        shdrpgm.Use();

        glBindVertexArray(Graphics::models[_spritecomponent->modelRef]->GetVaoID());

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, mode);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        CheckUniformLoc(&shdrpgm, *(_spritecomponent), ID, _camera);
        DrawIndexed(_spritecomponent, GL_UNSIGNED_SHORT);

        // Part 5: Clean up
        shdrpgm.UnUse();
        glBindVertexArray(0);
        //engine->gFrameBufferManager->UnBind(FrameBufferMode::FBM_SCENE);
    }

    void Eclipse::GraphicsManager::DrawIndexed(MeshComponent* in, GLenum mode)
    {
        (void)mode;

        GLCall(glDrawElements(Graphics::models[in->modelRef]->GetPrimitiveType(),
            Graphics::models[in->modelRef]->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));
    }

    void Eclipse::GraphicsManager::CheckTexture(unsigned int ID)
    {
        if (engine->world.CheckComponent<MaterialComponent>(ID))
        {
            MaterialComponent& tex = engine->world.GetComponent<MaterialComponent>(ID);

            if (tex.hasTexture && tex.TextureRef != "Default")
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, Graphics::FindTextures(tex.TextureRef).GetHandle());

                glEnable(GL_BLEND);

                glTextureParameteri(Graphics::FindTextures(tex.TextureRef).GetHandle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTextureParameteri(Graphics::FindTextures(tex.TextureRef).GetHandle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTextureParameteri(Graphics::FindTextures(tex.TextureRef).GetHandle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTextureParameteri(Graphics::FindTextures(tex.TextureRef).GetHandle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
            }
        }
    }

    void Eclipse::GraphicsManager::CheckUniformLoc(Shader* _shdrpgm, MeshComponent& sprite, unsigned int id, CameraComponent& camera)
    {
        TransformComponent camerapos;
        TransformComponent& trans = engine->world.GetComponent<TransformComponent>(id);

        GLint uniform_var_loc1 = _shdrpgm->GetLocation("uModelToNDC");
        GLint uniform_var_loc2 = _shdrpgm->GetLocation("uColor");
        GLint uniform_var_loc3 = _shdrpgm->GetLocation("uTextureCheck");
        GLuint tex_loc = _shdrpgm->GetLocation("uTex2d");
        GLuint model2 = _shdrpgm->GetLocation("model");
        GLuint noTex = _shdrpgm->GetLocation("noTex");
        GLuint CheckNormapMap = _shdrpgm->GetLocation("checkNormalMap");
        GLint uniform_var_loc10 = _shdrpgm->GetLocation("BasicPrimitives");

        glUniform1i(noTex, true);
        glUniform1i(CheckNormapMap, false);
        glUniform1i(uniform_var_loc10, true);


        glm::mat4 mModelNDC;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, trans.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(trans.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, trans.scale.ConvertToGlmVec3Type());
        mModelNDC = camera.projMtx * camera.viewMtx * model;
        glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));

        glUniform4f(uniform_var_loc2, sprite.color.getX(), sprite.color.getY(), sprite.color.getZ(), sprite.transparency);

        if (engine->world.CheckComponent<MaterialComponent>(id))
        {
            MaterialComponent& tex = engine->world.GetComponent<MaterialComponent>(id);
            glUniform1i(uniform_var_loc3, tex.hasTexture);
        }
        else
        {
            glUniform1i(uniform_var_loc3, false);
        }

        glUniform1i(tex_loc, false);
    }

    std::string Eclipse::GraphicsManager::GetModelName(unsigned int modelname)
    {
        switch (modelname)
        {
        case 0:
            return "Square";
            break;

        case 1:
            return "Straight_Stairs";
            break;

        case 2:
            return "Sphere";
            break;

        case 3:
            return "Plane";
            break;

        case 4:
            return "Cube";
            break;

        case 5:
            return "Cylinder";
            break;

        case 6:
            return "Cone";
            break;

        case 7:
            return "Torus";
            break;

        case 8:
            return "ICOSphere";
            break;

        case 9:
            return "Curved_Stairs";
            break;

        case 10:
            return "Spiral_Stairs";
            break;
        }

        return std::string();
    }

    void Eclipse::GraphicsManager::WindowCloseCallback(GLFWwindow* window)
    {
        (void)window;

        if (engine->GetEditorState())
        {
            if (!engine->editorManager->GetRecoveryFileExistence())
                engine->editorManager->GetMenuBar().SetExitStatus(true);

            glfwSetWindowShouldClose(OpenGL_Context::GetWindow(), GLFW_FALSE);
        }
        else
        {
            glfwSetWindowShouldClose(OpenGL_Context::GetWindow(), GLFW_TRUE);
        }
    }

    void Eclipse::GraphicsManager::SetBackGroundColour()
    {
        mRenderContext.SetClearColor({ BackGroundColour.getX(),  BackGroundColour.getY(),  BackGroundColour.getZ(), 1.0f });
    }

    void Eclipse::GraphicsManager::FinalRender()
    {
        if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
        {
            engine->GridManager->DrawGrid(FrameBufferMode::FBM_SCENE);
        }

        engine->gFrameBufferManager->PostProcess->UpdatePP();
    }

    void Eclipse::GraphicsManager::CreateCompilerFolders()
    {
        // Insurance to create files.
        CreateEmptyFolder("BasicTextureFile");
        CreateEmptyFolder("GeometryFile");
        CreateEmptyFolder("PrefabsFile");
        CreateEmptyFolder("TextureFile");
    }

    void GraphicsManager::CreateEmptyFolder(std::string folderName, std::string folderPath)
    {
        std::string fileLocation = folderPath + folderName;
        std::string outputLog;
        if (std::filesystem::exists(fileLocation) == false)
        {
            outputLog = "FOLDER CREATED AT: ";
            outputLog += fileLocation;
            std::filesystem::create_directories(fileLocation);
            EDITOR_LOG_INFO(outputLog.c_str());
        }
    }

    /*************************************************************************
      FrameBuffer Things
    *************************************************************************/

#ifndef FrameBuffer_Things

    float Eclipse::GraphicsManager::GetGammaCorrection()
    {
        return GammaCorrection;
    }

    void Eclipse::GraphicsManager::SetGammaCorrection(float in)
    {
        GammaCorrection = in;
    }

    void Eclipse::GraphicsManager::UploadGlobalUniforms()
    {
        CameraComponent camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetEditorCameraID());

        auto& shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        GLint uniform_var_loc1 = shdrpgm.GetLocation("gamma");
        GLint uniform_var_loc2 = shdrpgm.GetLocation("EnableGammaCorrection");
        GLint uniform_var_loc3 = shdrpgm.GetLocation("camPos");
        GLint uniform_var_loc4 = shdrpgm.GetLocation("Exposure");
        GLint uniform_var_loc5 = shdrpgm.GetLocation("EnableNormalMapping");

        GLCall(glUniform1f(uniform_var_loc1, engine->GraphicsManager.GetGammaCorrection()));
        GLCall(glUniform1i(uniform_var_loc2, engine->GraphicsManager.EnableGammaCorrection));
        GLCall(glUniform3f(uniform_var_loc3, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ()));
        GLCall(glUniform1f(uniform_var_loc4, engine->GraphicsManager.Exposure));
        GLCall(glUniform1i(uniform_var_loc5, engine->GraphicsManager.EnableNormalMapping));
        shdrpgm.UnUse();
    }
#endif
}