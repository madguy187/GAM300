#include "pch.h"
#include "Graphics/RendererAPI/GraphicsManager.h"
#include "EntryPoint/EntryPoint.h"

glm::vec3 spherepos;

void Eclipse::GraphicsManager::pre_render()
{
    // Loading Configuration
    mRenderContext.init("../Dep/Configuration/configuration.json");

    // Loading Of Models , Shaders and etc.. 
    Graphics::load();

    // Clear the View
    mRenderContext.pre_render();
}

void Eclipse::GraphicsManager::post_render()
{
    mRenderContext.post_render();
}

void Eclipse::GraphicsManager::end()
{
    mRenderContext.end();
}

void Eclipse::GraphicsManager::unload()
{
    mRenderContext.end();
}

void Eclipse::GraphicsManager::CreatePrimitives(GLint model)
{
    auto& testtest = engine->world;
    Entity EntityID = testtest.CreateEntity();
    createdID = EntityID;

    switch (model)
    {
    case 0:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("square");
        sprite.isQuad = true;
        sprite.hasTexture = true;
        sprite.textureRef = Graphics::textures.find("orange");
    }
    break;

    case 1:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("shdrpgm");
        sprite.modelRef = Graphics::models.find("circle");
    }
    break;
    case 2:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("shdrpgm");
        sprite.modelRef = Graphics::models.find("triangle");
    }
    break;
    case 3:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("shdrpgm");
        sprite.modelRef = Graphics::models.find("lines");
    }
    break;

    case 4:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("nooblight");
        sprite.modelRef = Graphics::models.find("sphere");
        sprite.ID = EntityID;
    }
    break;

    case 5:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("cube");
        sprite.hasTexture = true;
        sprite.textureRef = Graphics::textures.find("orange");
    }
    break;

    case 6:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("cylinder");
    }
    break;

    case 7:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("cone");
    }
    break;

    case 8:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("torus");
    }
    break;

    case 9:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("pyramid");
    }
    break;
    case 10:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("lines3D");
    }
    break;

    case 11:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("plane");
    }
    break;
    case 12:
    {
        engine->LightManager.CreatePointLight();
    }
    break;
    }
}

void Eclipse::GraphicsManager::DrawBuffers(FrameBuffer* in, RenderComponent* _spritecomponent, GLenum mode)
{
    in->Bind();

    // Part 1: Install the shader program used by this object to render its model using GLSLShader::Use()
    _spritecomponent->shaderRef->second.Use();

    // Part 2: Bind the object's VAO handle using glBindVertexArray
    GLCall(glBindVertexArray(_spritecomponent->modelRef->second->GetVaoID()));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthMask(GL_TRUE));
    GLCall(glDepthFunc(GL_LEQUAL));
    GLCall(glDepthRange(0.0f, 1.0f));
    GLCall(glShadeModel(GL_SMOOTH));
    GLCall(glDisable(GL_CULL_FACE));
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, mode));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    CheckHasTexture(_spritecomponent);
    CheckUniformLoc(*(_spritecomponent), _spritecomponent->ID, in->GetFrameBufferID());
    DrawIndexed(_spritecomponent, GL_UNSIGNED_SHORT);

    // Part 5: Clean up
    glBindVertexArray(0);
    _spritecomponent->shaderRef->second.UnUse();
}

void Eclipse::GraphicsManager::L_CheckUniformLoc(RenderComponent& sprite, unsigned int id, unsigned int framebufferID)
{
    CameraComponent camera;

    if (framebufferID == engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID())
    {
        camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
    }
    else
    {
        //Check if game camera exists
        if (engine->gCamera.GetGameCameraID() == MAX_ENTITY)
        {
            return;
        }

        camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetGameCameraID());
    }

    auto& camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetEditorCameraID());
    TransformComponent& trans = engine->world.GetComponent<TransformComponent>(id);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    GLint uniform_var_loc1 = sprite.shaderRef->second.GetLocation("uModelToNDC");
    GLuint lll = sprite.shaderRef->second.GetLocation("lightColor");

    spherepos = trans.pos;

    if (uniform_var_loc1 >= 0)
    {
        glm::mat4 mModelNDC;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, trans.pos);
        model = glm::rotate(model, glm::radians(trans.rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, trans.scale);
        mModelNDC = camera.projMtx * camera.viewMtx * model;
        glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mModelNDC));
    }

    if (lll >= 0)
    {
        glUniform4f(lll, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    }
}

void Eclipse::GraphicsManager::DrawIndexed(RenderComponent* _in, GLenum type)
{
    GLCall(glDrawElements(_in->modelRef->second->GetPrimitiveType(), _in->modelRef->second->GetDrawCount(), type, NULL));
}

void Eclipse::GraphicsManager::CheckHasTexture(RenderComponent* _check)
{
    if (_check->hasTexture)
    {
        if (_check->textureRef != Graphics::textures.end())
        {
            GLCall(glBindTextureUnit(1, _check->textureRef->second.GetHandle()));
            GLCall(glEnable(GL_BLEND));
            GLCall(glTextureParameteri(_check->textureRef->second.GetHandle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTextureParameteri(_check->textureRef->second.GetHandle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTextureParameteri(_check->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_S, GL_REPEAT));
            GLCall(glTextureParameteri(_check->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_T, GL_REPEAT));
        }
    }
}

void Eclipse::GraphicsManager::CheckUniformLoc(RenderComponent& sprite, unsigned int id, unsigned int framebufferID)
{
    CameraComponent camera;

    if (framebufferID == engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID())
    {
        camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
    }
    else
    {
        //Check if game camera exists
        if (engine->gCamera.GetGameCameraID() == MAX_ENTITY)
        {
            return;
        }

        camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetGameCameraID());
    }

    TransformComponent& camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetEditorCameraID());
    TransformComponent& trans = engine->world.GetComponent<TransformComponent>(id);

    GLint uniform_var_loc1 = sprite.shaderRef->second.GetLocation("uModelToNDC");
    GLint uniform_var_loc2 = sprite.shaderRef->second.GetLocation("uColor");
    GLint uniform_var_loc3 = sprite.shaderRef->second.GetLocation("uTextureCheck");
    GLint uniform_var_loc4 = sprite.shaderRef->second.GetLocation("TextureIndex");
    GLint uniform_var_loc5 = sprite.shaderRef->second.GetLocation("TextureDimensions");
    GLuint tex_loc = sprite.shaderRef->second.GetLocation("uTex2d");
    GLuint lll = sprite.shaderRef->second.GetLocation("lightColor");
    GLuint cam = sprite.shaderRef->second.GetLocation("camPos");
    GLuint pos = sprite.shaderRef->second.GetLocation("lightPos");
    GLuint model2 = sprite.shaderRef->second.GetLocation("model");

    if (uniform_var_loc1 >= 0)
    {
        glm::mat4 mModelNDC;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, trans.pos);
        model = glm::rotate(model, glm::radians(trans.rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, trans.scale);
        mModelNDC = camera.projMtx * camera.viewMtx * model;
        glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
    }

    if (pos >= 0)
    {
        glUniform3f(pos, spherepos.x, spherepos.y, spherepos.z);
    }

    if (cam >= 0)
    {
        glUniform3f(lll, camerapos.position.x, camerapos.position.y, camerapos.position.z);
    }

    if (lll >= 0)
    {
        glUniform4f(lll, sprite.lightColor.x, sprite.lightColor.y, sprite.lightColor.z, sprite.lightColor.w);
    }

    if (uniform_var_loc2 >= 0)
    {
        glUniform4f(uniform_var_loc2, sprite.color.x, sprite.color.y, sprite.color.z, sprite.transparency);
    }

    if (uniform_var_loc3 >= 0)
    {
        glUniform1i(uniform_var_loc3, sprite.hasTexture);
    }

    if (uniform_var_loc4 >= 0)
    {
        GLCall(glUniform2f(uniform_var_loc4, sprite.textureIdx.x, sprite.textureIdx.y));
    }

    if (sprite.hasTexture)
    {
        if (sprite.textureRef != Graphics::textures.end())
        {
            if (uniform_var_loc5 >= 0)
            {
                GLCall(glUniform2f(uniform_var_loc5, sprite.textureRef->second.GetCols(), sprite.textureRef->second.GetRows()));
            }
            else
            {
                std::cout << "Uniform variable doesn't exist!!!\n";
                std::exit(EXIT_FAILURE);
            }
        }
    }

    if (tex_loc >= 0)
    {
        glUniform1i(tex_loc, 1);
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
    // Will be Removed , its imgui stuffs
    Eclipse::GraphicsManager::ShowTestWidgets();

    FrameBuffer::ShowWindow(*(mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)), "GameView");
    FrameBuffer::ShowWindow(*(mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)), "SceneView");
}

#endif

/*************************************************************************
  ImGui Test Things
*************************************************************************/

#ifndef Imgui_Things

void Eclipse::GraphicsManager::ShowTestWidgets()
{
    int modelSelector = -1;
    ImGui::Begin("Create Objects");

    if (ImGui::Combo("Models", &modelSelector,
        "Square\0Circle\0Triangle\0Lines\0Sphere\0Cube\0Cylinder\0Cone\0Torus\0Pyramid\0Lines3D\0Plane\0testlight"))
    {
        CreatePrimitives(modelSelector);

        for (int i = 0; i <= 2; ++i)
        {
            ImGui::Separator();
        }
    }
    ImGui::End();
}

void Eclipse::GraphicsManager::ShowTestWidgets(unsigned int id, unsigned int createdId)
{
    if (id == createdId)
    {
        TransformComponent& trans = engine->world.GetComponent<TransformComponent>(id);
        ImGui::DragFloat3("Scale", (float*)&trans.scale, 0.2f, 0.0f, 0.0f);
        ImGui::DragFloat3("Translate", (float*)&trans.pos, 0.2f, 0.0f, 0.0f);
        ImGui::DragFloat3("Rotate", (float*)&trans.rot, 0.2f, 0.0f, 0.0f);
    }
}

#endif
