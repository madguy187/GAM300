#include "pch.h"
#include "Graphics/RendererAPI/GraphicsManager.h"
#include "EntryPoint/EntryPoint.h"

float shakeTimer = 1.0f;
bool shakeScreen = 1.0f;

glm::vec3 spherepos;

void Eclipse::GraphicsManager::pre_render()
{  // Loading Configuration
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
    /*ImGui::DestroyContext();*/
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
        //Graphics::sprites.emplace(sprite.layerNum, &sprite);
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
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
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
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
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
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
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
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
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
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
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
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
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
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
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
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    case 12:
    {
        testtest.AddComponent(EntityID, RenderComponent{ });
        testtest.AddComponent(EntityID, TransformComponent{ });
        RenderComponent& sprite = engine->world.GetComponent<RenderComponent>(EntityID);
        sprite.ID = EntityID;
        sprite.shaderRef = Graphics::shaderpgms.find("nooblight");
        sprite.modelRef = Graphics::models.find("cube");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;
    }
}

void Eclipse::GraphicsManager::DrawBuffers(unsigned int FrameBufferID, RenderComponent* _spritecomponent, GLenum mode)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

    // Part 1: Install the shader program used by this object to render its model using GLSLShader::Use()
    _spritecomponent->shaderRef->second.Use();

    // Part 2: Bind the object's VAO handle using glBindVertexArray
    glBindVertexArray(_spritecomponent->modelRef->second->GetVaoID());

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (_spritecomponent->hasTexture)
    {
        if (_spritecomponent->textureRef != Graphics::textures.end())
        {
            glBindTextureUnit(1, _spritecomponent->textureRef->second.GetHandle());

            glEnable(GL_BLEND);

            glTextureParameteri(_spritecomponent->textureRef->second.GetHandle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(_spritecomponent->textureRef->second.GetHandle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(_spritecomponent->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(_spritecomponent->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
    }

    if (_spritecomponent->ID == 2)
    {
        CheckUniformLoc(*(_spritecomponent), _spritecomponent->ID, FrameBufferID);
    }
    else if (_spritecomponent->ID == 3)
    {
        L_CheckUniformLoc(*(_spritecomponent), _spritecomponent->ID, FrameBufferID);
    }

    glDrawElements(_spritecomponent->modelRef->second->GetPrimitiveType(), _spritecomponent->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL);

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

void Eclipse::GraphicsManager::CheckUniformLoc(RenderComponent& sprite, unsigned int id, unsigned int framebufferID)
{
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
        //GLint uniform_var_loc6 = sprite.shaderRef->second.GetLocation("LightTimer");
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

void Eclipse::GraphicsManager::ImguiRender()
{
    // To be Removed
    /*int Width, Height;
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwGetFramebufferSize(mRenderContext.GetWindow(), &Width, &Height);
    mRenderContext.SetViewport(0, 0, Width, Height);*/
    //mRenderContext.SetClearColor({ 0.1f, 0.2f, 0.3f, 1.f });
}

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
