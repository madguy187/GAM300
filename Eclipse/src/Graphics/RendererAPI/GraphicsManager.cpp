#include "pch.h"
#include "Graphics/RendererAPI/GraphicsManager.h"
#include "EntryPoint/EntryPoint.h"

float shakeTimer = 1.0f;
bool shakeScreen = 1.0f;

glm::vec3 spherepos;

void Eclipse::GraphicsManager::Pre_Render()
{  
    // Loading Configuration
    mRenderContext.init("../Dep/Configuration/configuration.json");

    // Loading Of Models , Shaders and etc.. 
    Graphics::load();

    // Clear the View
    mRenderContext.pre_render();


    // Initialise Grid
    GridManager = std::make_unique<Grid>();
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
        engine->world.AddComponent(ID, RenderComponent{});
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("square");
        sprite.isQuad = true;
    }
    break;
    case 1:
    {
        engine->world.AddComponent(ID, RenderComponent{});
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shdrpgm");
        sprite.modelRef = Graphics::models.find("circle");
    }
    break;
    case 2:
    {
        engine->world.AddComponent(ID, RenderComponent{});
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shdrpgm");
        sprite.modelRef = Graphics::models.find("triangle");
    }
    break;
    case 3:
    {
        engine->world.AddComponent(ID, RenderComponent{});
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shdrpgm");
        sprite.modelRef = Graphics::models.find("lines");
    }
    break;
    case 4:
    {
        engine->world.AddComponent(ID, RenderComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("sphere");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 5:
    {
        engine->world.AddComponent(ID, RenderComponent{});
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("cube");

        sprite.hasTexture = true;
        sprite.textureRef = Graphics::textures.find("orange");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 6:
    {
        engine->world.AddComponent(ID, RenderComponent{});
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("cylinder");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 7:
    {
        engine->world.AddComponent(ID, RenderComponent{});
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("cone");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 8:
    {
        engine->world.AddComponent(ID, RenderComponent{});
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("torus");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 9:
    {
        engine->world.AddComponent(ID, RenderComponent{});
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("pyramid");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 10:
    {
        engine->world.AddComponent(ID, RenderComponent{});
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("lines3D");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 11:
    {
        engine->world.AddComponent(ID, RenderComponent{});
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("plane");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    // pointlight
    case 12:
    {
        // Testing of creating prefabs is fine.
        engine->AssimpManager.CreateModel("White Dog", "dog" , "scene.gltf");
        
        //engine->LightManager.CreateLights(Eclipse::TypesOfLights::POINTLIGHT, ID);
    }
    break;
    // Directional
    case 13:
    {
        engine->LightManager.CreateLights(Eclipse::TypesOfLights::DIRECTIONAL, ID);
    }
    break;
    // SpotLight
    case 14:
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

void Eclipse::GraphicsManager::RenderSky(unsigned int FrameBufferID)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
    auto shdrpgm = Graphics::shaderpgms.find("Sky");

    Sky->Render(shdrpgm->second);

    //shdrpgm->second.UnUse();
}

void Eclipse::GraphicsManager::Draw(unsigned int FrameBufferID, RenderComponent* _spritecomponent, GLenum mode)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

    // Part 1: Install the shader program used by this object to render its model using GLSLShader::Use()
    _spritecomponent->shaderRef->second.Use();

    // Part 2: Bind the object's VAO handle using glBindVertexArray
    glBindVertexArray(_spritecomponent->modelRef->second->GetVaoID());

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CheckTexture(_spritecomponent);
    CheckUniformLoc(*(_spritecomponent), _spritecomponent->ID, FrameBufferID);
    DrawIndexed(_spritecomponent, GL_UNSIGNED_SHORT);

    // Part 5: Clean up
    glBindVertexArray(0);
    _spritecomponent->shaderRef->second.UnUse();
}

void Eclipse::GraphicsManager::DrawIndexed(RenderComponent* in, GLenum mode)
{
    glDrawElements(in->modelRef->second->GetPrimitiveType(), in->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL);
}

void Eclipse::GraphicsManager::CheckTexture(RenderComponent* in)
{
    if (in->hasTexture)
    {
        if (in->textureRef != Graphics::textures.end())
        {
            glBindTexture(GL_TEXTURE_2D, in->textureRef->second.GetHandle());

            glEnable(GL_BLEND);

            glTextureParameteri(in->textureRef->second.GetHandle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(in->textureRef->second.GetHandle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(in->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(in->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
    }
}

void Eclipse::GraphicsManager::CheckUniformLoc(RenderComponent& sprite, unsigned int id, unsigned int framebufferID)
{
    CameraComponent camera;
    TransformComponent camerapos;

    if (framebufferID == engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID())
    {
        camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
        camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetGameCameraID());
    }
    else
    {
        //Check if game camera exists
        if (engine->gCamera.GetGameCameraID() == MAX_ENTITY)
        {
            return;
        }

        camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetGameCameraID());
        camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetGameCameraID());
    }

    TransformComponent& trans = engine->world.GetComponent<TransformComponent>(id);

    GLint uniform_var_loc1 = sprite.shaderRef->second.GetLocation("uModelToNDC");
    GLint uniform_var_loc2 = sprite.shaderRef->second.GetLocation("uColor");
    GLint uniform_var_loc3 = sprite.shaderRef->second.GetLocation("uTextureCheck");
    //GLint uniform_var_loc4 = sprite.shaderRef->second.GetLocation("TextureIndex");
    //GLint uniform_var_loc5 = sprite.shaderRef->second.GetLocation("TextureDimensions");
    GLuint tex_loc = sprite.shaderRef->second.GetLocation("uTex2d");
    GLuint cam = sprite.shaderRef->second.GetLocation("camPos");
    GLuint model2 = sprite.shaderRef->second.GetLocation("model");
    GLuint dsa = sprite.shaderRef->second.GetLocation("noTex");
    GLuint TEST = sprite.shaderRef->second.GetLocation("TEST");

    // I will need to change all these with Material system
    // ------------------------------------------------------------
    GLuint aa = sprite.shaderRef->second.GetLocation("sdiffuse");
    GLuint bb = sprite.shaderRef->second.GetLocation("sspecular");

    //if (dsa >= 0)
    //{
    //    glUniform1i(dsa, true);
    //    glUniform4f(aa, 1, 1, 1, 1);
    //    glUniform4f(bb, 1, 1, 1, 1);
    //}

    // ------------------------------------------------------------


    if (TEST >= 0)
    {
        glUniform1i(TEST, 1);
    }

    if (uniform_var_loc1 >= 0)
    {
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
    }

    if (cam >= 0)
    {
        glUniform3f(cam, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ());
    }

    if (uniform_var_loc2 >= 0)
    {
        glUniform4f(uniform_var_loc2, sprite.color.getX(), sprite.color.getY(), sprite.color.getZ(), sprite.transparency);
    }

    if (uniform_var_loc3 >= 0)
    {
        glUniform1i(uniform_var_loc3, sprite.hasTexture);
    }

    //if (uniform_var_loc4 >= 0)
    //{
    //    GLCall(glUniform2f(uniform_var_loc4, sprite.textureIdx.getX(), sprite.textureIdx.getY()));
    //}

    //if (sprite.hasTexture)
    //{
    //    if (sprite.textureRef != Graphics::textures.end())
    //    {
    //        if (uniform_var_loc5 >= 0)
    //        {
    //            GLCall(glUniform2f(uniform_var_loc5, sprite.textureRef->second.GetCols(), sprite.textureRef->second.GetRows()));
    //        }
    //        else
    //        {
    //            std::cout << "Uniform variable doesn't exist!!!\n";
    //            std::exit(EXIT_FAILURE);
    //        }
    //    }
    //}

    if (tex_loc >= 0)
    {
        glUniform1i(tex_loc, false);
    }
}

/*************************************************************************
  FrameBuffer Things
*************************************************************************/

#ifndef FrameBuffer_Things

void Eclipse::GraphicsManager::UpdateFrameBuffer()
{
    for (auto const& SelectedFrameBuffer : mRenderContext._Framebuffers)
    {
        SelectedFrameBuffer.second->Bind();
        SelectedFrameBuffer.second->Clear();
    }
}

void Eclipse::GraphicsManager::DebugPrintFrameBuffers()
{
    std::cout << "FrameBuffer Size :" << mRenderContext._Framebuffers.size() << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    for (auto const& SelectedFrameBuffer : mRenderContext._Framebuffers)
    {
        std::cout << " FrameBuffer Name :" << SelectedFrameBuffer.second->GetName() << std::endl;
        std::cout << " FrameBuffer ID :" << SelectedFrameBuffer.second->GetFrameBufferID() << std::endl;
        std::cout << " DepthBuffer ID :" << SelectedFrameBuffer.second->GetDepthBufferID() << std::endl;
        std::cout << " ColorBuffer ID :" << SelectedFrameBuffer.second->GetTextureColourBufferID() << std::endl;
        std::cout << " Type  : " << SelectedFrameBuffer.second->GetFrameBufferType() << std::endl;
        std::cout << std::endl;
    }

    ENGINE_CORE_INFO("All FrameBufers Created");
}

float Eclipse::GraphicsManager::GetGammaCorrection()
{
    return GammaCorrection;
}

void Eclipse::GraphicsManager::SetGammaCorrection(float in)
{
    GammaCorrection = in;
}

void Eclipse::GraphicsManager::UploadGammaCorrectionToShader()
{
    auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");
    shdrpgm->second.Use();

    GLint uniform_var_loc1 = shdrpgm->second.GetLocation("gamma");

    if (uniform_var_loc1 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc1, engine->GraphicsManager.GetGammaCorrection()));
    }

    shdrpgm->second.UnUse();
}

void Eclipse::GraphicsManager::GlobalFrameBufferBind()
{
    Eclipse::GraphicsManager::UpdateFrameBuffer();
}

FrameBuffer* Eclipse::OpenGL_Context::GetFramebuffer(FrameBufferMode mode)
{
    if (mode == FrameBufferMode::MAXCOUNT || mode == FrameBufferMode::NONE)
    {
        ENGINE_LOG_ASSERT(false, " Wrong FrameBuffer Type");
        std::exit(EXIT_FAILURE);
        return nullptr;
    }

    for (auto const& it : _Framebuffers)
    {
        auto& type = (it.first);
        auto& selectfb = *(it.second);

        if (type == mode)
        {
            return &selectfb;
        }
    }

    return nullptr;
}

void Eclipse::GraphicsManager::FrameBufferDraw()
{
    FrameBuffer::ShowWindow(*(mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)), "GameView");
    FrameBuffer::ShowWindow(*(mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)), "SceneView");
    FrameBuffer::ShowWindow(*(mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_TOP)), "SceneView_Top");
    FrameBuffer::ShowWindow(*(mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_BOTTOM)), "SceneView_Bottom");
    FrameBuffer::ShowWindow(*(mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_LEFT)), "SceneView_Left");
    FrameBuffer::ShowWindow(*(mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_RIGHT)), "SceneView_Right");
}

void Eclipse::GraphicsManager::GlobalFrmeBufferDraw()
{
    Eclipse::GraphicsManager::FrameBufferDraw();
}

#endif
