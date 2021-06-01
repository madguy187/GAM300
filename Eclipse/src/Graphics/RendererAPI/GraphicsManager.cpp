#include "pch.h"
#include "Graphics/RendererAPI/GraphicsManager.h"
#include "EntryPoint/EntryPoint.h"

float shakeTimer = 1.0f;
bool shakeScreen = 1.0f;

void Eclipse::GraphicsManager::pre_render()
{  // Loading Configuration
    mRenderContext.init("../Dep/Configuration/configuration.json");

    // Loading Of Models , Shaders and etc.. 
    Graphics::load();

    // To be Removed
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(mRenderContext.GetWindow(), true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsClassic();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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
    ImGui::DestroyContext();
}

void Eclipse::GraphicsManager::CreatePrimitives(GLint model)
{
    auto& testtest = engine->world;
    Entity EntityID = testtest.CreateEntity();
    createdID = EntityID;
    testtest.AddComponent(EntityID, Sprite{ });
    testtest.AddComponent(EntityID, TransformComponent{ });

    Sprite& sprite = engine->world.GetComponent<Sprite>(EntityID);
    sprite.layerNum = 10;
    sprite.ID = EntityID;

    switch (model)
    {
    case 0:
    {
        sprite.shaderRef = Graphics::shaderpgms.find("shdrpgm");
        sprite.modelRef = Graphics::models.find("square");
        sprite.isQuad = true;
    }
    break;

    case 1:
        sprite.shaderRef = Graphics::shaderpgms.find("shdrpgm");
        sprite.modelRef = Graphics::models.find("circle");
        break;
    case 2:
    {
        sprite.shaderRef = Graphics::shaderpgms.find("shdrpgm");
        sprite.modelRef = Graphics::models.find("triangle");
    }
    break;
    case 3:
    {
        sprite.shaderRef = Graphics::shaderpgms.find("shdrpgm");
        sprite.modelRef = Graphics::models.find("lines");
    }
    break;

    case 4:
    {
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("sphere");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
        sprite.ID = EntityID;
    }
    break;

    case 5:
    {
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("cube");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;

    case 6:
    {
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("cylinder");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;

    case 7:
    {
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("cone");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;

    case 8:
    {
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("torus");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;

    case 9:
    {
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("pyramid");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
    break;

    case 10:
        sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
        sprite.modelRef = Graphics::models.find("lines3D");
        Graphics::sprites.emplace(sprite.layerNum, &sprite);
    }
}

void Eclipse::GraphicsManager::DrawBuffers(unsigned int FrameBufferID, Sprite* _spritecomponent, GLenum mode)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

    // Part 1: Install the shader program used by this object to render its model using GLSLShader::Use()
    _spritecomponent->shaderRef->second.Use();

    // Part 2: Bind the object's VAO handle using glBindVertexArray
    glBindVertexArray(_spritecomponent->modelRef->second->GetVaoID());

    glEnable(GL_BLEND);

    if (_spritecomponent->modelRef == Graphics::FindModel("sphere") ||
        _spritecomponent->modelRef == Graphics::FindModel("cube") ||
        _spritecomponent->modelRef == Graphics::FindModel("cone") ||
        _spritecomponent->modelRef == Graphics::FindModel("torus") ||
        _spritecomponent->modelRef == Graphics::FindModel("pyramid") ||
        _spritecomponent->modelRef == Graphics::FindModel("cylinder") ||
        _spritecomponent->modelRef == Graphics::FindModel("lines3D"))
    {

        glPolygonMode(GL_FRONT_AND_BACK, mode);
        glDisable(GL_CULL_FACE);
        glEnable(GL_LINE_SMOOTH);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_CULL_FACE);
    }

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

    CheckUniformLoc(*(_spritecomponent), _spritecomponent->ID);

    if (_spritecomponent->modelRef->second->GetPrimitiveType() != GL_LINES)
    {
        glDrawElements(_spritecomponent->modelRef->second->GetPrimitiveType(),
            _spritecomponent->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL);
    }
    else
    {
        glLineWidth(5.0f); //Note that glLineWidth() is deprecated
        glDrawArrays(_spritecomponent->modelRef->second->GetPrimitiveType(), 0, 2);
    }

    // Part 5: Clean up
    glBindVertexArray(0);
    _spritecomponent->shaderRef->second.UnUse();
}

void Eclipse::GraphicsManager::CheckUniformLoc(Sprite& sprite, unsigned int id)
{
    {
        unsigned int currCamID = 0;
        //Camera& oi = engine->world.GetComponent<Camera>(currCamID);
        auto& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
        TransformComponent& trans = engine->world.GetComponent<TransformComponent>(id);

        GLint uniform_var_loc1 = sprite.shaderRef->second.GetLocation("uModelToNDC");
        GLint uniform_var_loc2 = sprite.shaderRef->second.GetLocation("uColor");
        GLint uniform_var_loc3 = sprite.shaderRef->second.GetLocation("uTextureCheck");
        GLint uniform_var_loc4 = sprite.shaderRef->second.GetLocation("TextureIndex");
        GLint uniform_var_loc5 = sprite.shaderRef->second.GetLocation("TextureDimensions");
        GLint uniform_var_loc6 = sprite.shaderRef->second.GetLocation("LightTimer");
        GLint uniform_var_loc7 = sprite.shaderRef->second.GetLocation("ShakeTimer");
        GLint uniform_var_loc8 = sprite.shaderRef->second.GetLocation("ShakeScreen");
        GLuint tex_loc = sprite.shaderRef->second.GetLocation("uTex2d");

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

        if (uniform_var_loc2 >= 0)
        {
            //float alpha = 1.0f;
            glUniform4f(uniform_var_loc2, sprite.color.x, sprite.color.y, sprite.color.z, sprite.transparency);
        }

        if (uniform_var_loc3 >= 0)
        {
            glUniform1i(uniform_var_loc3, sprite.hasTexture);
        }

        if (uniform_var_loc4 >= 0)
        {
            glUniform2f(uniform_var_loc4, sprite.textureIdx.x, sprite.textureIdx.y);
        }

        if (sprite.hasTexture)
        {
            if (sprite.textureRef != Graphics::textures.end())
            {
                if (uniform_var_loc5 >= 0)
                {
                    glUniform2f(uniform_var_loc5, sprite.textureRef->second.GetCols(), sprite.textureRef->second.GetRows());
                }
                else
                {
                    std::cout << "Uniform variable doesn't exist!!!\n";
                    std::exit(EXIT_FAILURE);
                }
            }
        }

        if (uniform_var_loc7 >= 0)
        {
            glUniform1f(uniform_var_loc7, shakeTimer);
        }

        if (uniform_var_loc8 >= 0)
        {
            glUniform1i(uniform_var_loc8, shakeScreen);
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
        ENGINE_CORE_INFO("Wrong FrameBuffer Type");
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
    int Width, Height;
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwGetFramebufferSize(mRenderContext.GetWindow(), &Width, &Height);
    mRenderContext.SetViewport(0, 0, Width, Height);
    mRenderContext.SetClearColor({ 0.1f, 0.2f, 0.3f, 1.f });
}

void Eclipse::GraphicsManager::ShowTestWidgets()
{
    int modelSelector = -1;
    ImGui::Begin("Create Objects");

    if (ImGui::Combo("Models", &modelSelector,
        "Square\0Circle\0Triangle\0Lines\0Sphere\0Cube\0Cylinder\0Cone\0Torus\0Pyramid\0Lines3D"))
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