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
        engine->world.AddComponent(ID, MaterialComponent{});
        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        engine->world.AddComponent(ID, MeshComponent{});
        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("Square")->first;
        sprite.isQuad = true;

    }
    break;
    case 1:
    {
        //engine->world.AddComponent(ID, TextureComponent{});
        engine->world.AddComponent(ID, MaterialComponent{});
        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        engine->world.AddComponent(ID, MeshComponent{});
        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("Circle")->first;
    }
    break;
    case 2:
    {
        engine->world.AddComponent(ID, MaterialComponent{});
        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        engine->world.AddComponent(ID, MeshComponent{});
        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("Triangle")->first;
    }
    break;
    case 3:
    {
        engine->world.AddComponent(ID, MaterialComponent{});
        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        engine->world.AddComponent(ID, MeshComponent{});
        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("Lines")->first;
    }
    break;
    case 4:
    {
        engine->world.AddComponent(ID, TextureComponent{});
        TextureComponent& text = engine->world.GetComponent<TextureComponent>(ID);

        engine->world.AddComponent(ID, MaterialComponent{});
        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        engine->world.AddComponent(ID, MeshComponent{});
        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("Sphere")->first;
        Graphics::sprites.emplace(sprite.layerNum, &sprite);

        text.hasTexture = true;
        text.textureRef = Graphics::textures.find("orange")->first;
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 5:
    {
        engine->world.AddComponent(ID, TextureComponent{});
        TextureComponent& text = engine->world.GetComponent<TextureComponent>(ID);

        engine->world.AddComponent(ID, MeshComponent{});
        engine->world.AddComponent(ID, MaterialComponent{});

        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("Cube")->first;
        text.hasTexture = true;
        text.textureRef = Graphics::textures.find("orange")->first;
        //Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 6:
    {
        engine->world.AddComponent(ID, MaterialComponent{});
        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        engine->world.AddComponent(ID, MeshComponent{});
        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("Cylinder")->first;
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 7:
    {
        engine->world.AddComponent(ID, MaterialComponent{});
        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        engine->world.AddComponent(ID, MeshComponent{});
        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("Cone")->first;
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 8:
    {
        engine->world.AddComponent(ID, MaterialComponent{});
        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        engine->world.AddComponent(ID, MeshComponent{});
        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("Torus")->first;
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 9:
    {
        engine->world.AddComponent(ID, MaterialComponent{});
        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        engine->world.AddComponent(ID, MeshComponent{});
        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("pyramid")->first;
        Graphics::sprites.emplace(sprite.layerNum, &sprite);

    }
    break;
    case 10:
    {
        engine->world.AddComponent(ID, MaterialComponent{});
        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        engine->world.AddComponent(ID, MeshComponent{});
        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("lines3D")->first;
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 11:
    {
        engine->world.AddComponent(ID, MaterialComponent{});
        MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
        mat.Modeltype = MaterialComponent::ModelType::BasicPrimitives;

        engine->world.AddComponent(ID, MeshComponent{});
        MeshComponent& sprite = engine->world.GetComponent<MeshComponent>(ID);
        sprite.ID = ID;
        sprite.shaderRef = (Graphics::shaderpgms.find("shader3DShdrpgm")->first);
        sprite.modelRef = Graphics::models.find("plane")->first;
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    // pointlight
    case 12:
    {
        engine->LightManager.CreateLights(Eclipse::TypesOfLights::POINTLIGHT, ID);
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
        //engine->LightManager.CreateLights(Eclipse::TypesOfLights::SPOTLIGHT, ID);

        engine->world.AddComponent(ID, ModeLInforComponent{});
        engine->world.AddComponent(ID, MaterialComponent{ MaterialComponent::ModelType::Models3D });
        engine->world.AddComponent(ID, MeshComponent{ engine->AssimpManager.GetKey("dog") });
        engine->world.AddComponent(ID, TextureComponent{});
        engine->AssimpManager.InsertModel(ID);
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
    if (DrawSky == true)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
        auto& shdrpgm = Graphics::shaderpgms["Sky"];

        Sky->Render(shdrpgm);
    }
}

void Eclipse::GraphicsManager::Draw(unsigned int FrameBufferID, MeshComponent* _spritecomponent, GLenum mode, unsigned int ID)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

    auto& shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];
    shdrpgm.Use();

    glBindVertexArray(Graphics::models[_spritecomponent->modelRef]->GetVaoID());

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CheckTexture(ID);
    CheckUniformLoc(&shdrpgm, *(_spritecomponent), _spritecomponent->ID, FrameBufferID);
    DrawIndexed(_spritecomponent, GL_UNSIGNED_SHORT);

    // Part 5: Clean up
    glBindVertexArray(0);
    shdrpgm.UnUse();
}

void Eclipse::GraphicsManager::DrawIndexed(MeshComponent* in, GLenum mode)
{
    GLCall(glDrawElements(Graphics::models[in->modelRef]->GetPrimitiveType(),
        Graphics::models[in->modelRef]->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));
}

void Eclipse::GraphicsManager::CheckTexture(unsigned int ID)
{
    if (engine->world.CheckComponent<TextureComponent>(ID))
    {
        TextureComponent& tex = engine->world.GetComponent<TextureComponent>(ID);

        if (tex.hasTexture)
        {

            glBindTexture(GL_TEXTURE_2D, Graphics::textures[tex.textureRef].GetHandle());

            glEnable(GL_BLEND);

            glTextureParameteri(Graphics::textures[tex.textureRef].GetHandle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(Graphics::textures[tex.textureRef].GetHandle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(Graphics::textures[tex.textureRef].GetHandle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(Graphics::textures[tex.textureRef].GetHandle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
    }
}

void Eclipse::GraphicsManager::CheckUniformLoc(Shader* _shdrpgm, MeshComponent& sprite, unsigned int id, unsigned int framebufferID)
{
    CameraComponent camera;
    TransformComponent camerapos;

    if (framebufferID == engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID())
    {
        camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
        camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetEditorCameraID());
    }
    else
    {
        //Check if game camera exists
        if (engine->gCamera.GetGameCameraID() == MAX_ENTITY)
        {
            return;
        }

        camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
        camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetEditorCameraID());
    }

    TransformComponent& trans = engine->world.GetComponent<TransformComponent>(id);

    GLint uniform_var_loc1 = _shdrpgm->GetLocation("uModelToNDC");
    GLint uniform_var_loc2 = _shdrpgm->GetLocation("uColor");
    GLint uniform_var_loc3 = _shdrpgm->GetLocation("uTextureCheck");
    //GLint uniform_var_loc4 = sprite.shaderRef->second.GetLocation("TextureIndex");
    //GLint uniform_var_loc5 = sprite.shaderRef->second.GetLocation("TextureDimensions");
    GLuint tex_loc = _shdrpgm->GetLocation("uTex2d");
    GLuint cam = _shdrpgm->GetLocation("camPos");
    GLuint model2 = _shdrpgm->GetLocation("model");

    GLuint hi = _shdrpgm->GetLocation("noTex");
    glUniform1i(hi, true);

    GLint uniform_var_loc10 = _shdrpgm->GetLocation("BasicPrimitives");
    glUniform1i(uniform_var_loc10, true);

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

    if (engine->world.CheckComponent<TextureComponent>(id))
    {
        TextureComponent& tex = engine->world.GetComponent<TextureComponent>(id);
        glUniform1i(uniform_var_loc3, tex.hasTexture);
    }
    else
    {
        glUniform1i(uniform_var_loc3, false);
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

void Eclipse::GraphicsManager::ResetInstancedDebugBoxes()
{
    AllAABBs.Reset();
}

void Eclipse::GraphicsManager::DrawDebugBoxes()
{
    // render boxes
    engine->GraphicsManager.AllAABBs.DrawAll(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());
}

std::string Eclipse::GraphicsManager::GetModelName(unsigned int modelname)
{
    switch (modelname)
    {
    case 0:
        return "Square";
        break;

    case 1:
        return "Triangle";
        break;

    case 2:
        return "Circle";
        break;

    case 3:
        return "Lines";
        break;

    case 4:
        return "Lightsquare";
        break;

    case 5:
        return "Sphere";
        break;

    case 6:
        return "Plane";
        break;

    case 7:
        return "Cube";
        break;

    case 8:
        return "Cylinder";
        break;

    case 9:
        return "Cone";
        break;

    case 10:
        return "Torus";
        break;

    case 11:
        return "Pyramid";
        break;

    case 12:
        return "Lines3D";
        break;

    case 13:
        return "Frustum";
        break;
    }

    return std::string();
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

void Eclipse::GraphicsManager::UploadGlobalUniforms()
{
    CameraComponent camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
    TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetGameCameraID());

    auto& shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];
    shdrpgm.Use();

    GLint uniform_var_loc1 = shdrpgm.GetLocation("gamma");
    GLint uniform_var_loc2 = shdrpgm.GetLocation("EnableGammaCorrection");
    GLint uniform_var_loc3 = shdrpgm.GetLocation("camPos");

    GLCall(glUniform1f(uniform_var_loc1, engine->GraphicsManager.GetGammaCorrection()));
    GLCall(glUniform1i(uniform_var_loc2, engine->GraphicsManager.EnableGammaCorrection));
    GLCall(glUniform3f(uniform_var_loc3, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ()));
    shdrpgm.UnUse();
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

    auto& selectfb = _Framebuffers[mode];

    return selectfb;
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
