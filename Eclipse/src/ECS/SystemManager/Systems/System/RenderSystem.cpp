#include "pch.h"
#include "RenderSystem.h"
#include "InputWrapper.h"
#include "AllInputKeyCodes.h"

float shakeTimer = 1.5f;
bool shakeScreen = false;

glm::vec3 spherePos = glm::vec3{ 0.0f, 0.0f, -10.0f };
glm::vec3 sphereScale = glm::vec3{ 5.0f, 5.0f, 5.0f };
glm::vec3 sphereRot = glm::vec3{ 0.0f, 0.0f, 0.0f };

void Eclipse::RenderSystem::Load()
{
  if (!GLHelper::init("Configuration/configuration.json"))
  {
    std::cout << "Unable to create OpenGL context" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  Graphics::load();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui_ImplGlfw_InitForOpenGL(GLHelper::ptr_window, true);
  ImGui_ImplOpenGL3_Init();
  ImGui::StyleColorsClassic();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  Graphics::init();
}

void Eclipse::RenderSystem::Init()
{

}

void Eclipse::RenderSystem::Render()
{

}

void Eclipse::RenderSystem::Update()
{
  //float fixedDeltaTime = 1.0 / 60.0;

  //ImGui_ImplOpenGL3_NewFrame();
  //ImGui_ImplGlfw_NewFrame();
  //ImGui::NewFrame();

  //Graphics::update(fixedDeltaTime);

  //std::stringstream sstr;
  //sstr << std::fixed << std::setprecision(2) << GLHelper::title << " | FPS: " << GLHelper::fps;
  //glfwSetWindowTitle(GLHelper::ptr_window, sstr.str().c_str());

  //Graphics::m_frameBuffer->Bind();
  //Graphics::m_frameBuffer->Clear();

  //glBindFramebuffer(GL_FRAMEBUFFER, Graphics::framebuffer);
  //glClear(GL_COLOR_BUFFER_BIT);

  //InputHandler.SetIsPrint(true);

  //if (InputHandler.GetKeyTriggered(InputKeycode::Key_LEFT))
  //{
  //  std::cout << " Move Left " << std::endl;
  //}
  //else if (InputHandler.GetKeyTriggered(InputKeycode::Key_RIGHT))
  //{
  //  std::cout << " MOVE RIGHT " << std::endl;
  //}
  //else if (InputHandler.GetKeyTriggered(InputKeycode::Key_SPACE))
  //{
  //  std::cout << " JUMP" << std::endl;
  //}
  //else if (InputHandler.GetKeyTriggered(InputKeycode::Key_UP))
  //{
  //  std::cout << " MOVE UP " << std::endl;
  //}
  //else if (InputHandler.GetKeyCurrent(InputKeycode::Key_5))
  //{
  //  std::cout << " Hold Apple 5 " << std::endl;
  //}
  //else if (InputHandler.GetKeyTriggered(InputKeycode::Key_ESC))
  //{
  //  glfwSetWindowShouldClose(GLHelper::ptr_window, GLFW_TRUE);
  //}

  //DrawSecondBuffers(Graphics::framebuffer);
  //DrawBuffers(Graphics::m_frameBuffer->GetGameViewBuffer());

  //Graphics::draw();
  //ImGui::Render();

  //int Width, Height;
  //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  //glfwGetFramebufferSize(GLHelper::ptr_window, &Width, &Height);
  //glViewport(0, 0, Width, Height);
  //glfwSwapBuffers(GLHelper::ptr_window);
  //glfwPollEvents();
}

void Eclipse::RenderSystem::unLoad()
{
  //ImGui::DestroyContext();
  //GLHelper::cleanup();
}

void Eclipse::RenderSystem::CheckUniformLoc(Sprite& sprite)
{
  {
    unsigned int currCamID = 0;

    Camera& oi = engine->world.GetComponent<Camera>(currCamID);

    for (auto const& entity : mEntities)
    {
      TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);

      GLint uniform_var_loc1 = glGetUniformLocation(sprite.shaderRef->second.GetHandle(), "uModelToNDC");
      GLint uniform_var_loc2 = glGetUniformLocation(sprite.shaderRef->second.GetHandle(), "uColor");
      GLint uniform_var_loc3 = glGetUniformLocation(sprite.shaderRef->second.GetHandle(), "uTextureCheck");
      GLint uniform_var_loc4 = glGetUniformLocation(sprite.shaderRef->second.GetHandle(), "TextureIndex");
      GLint uniform_var_loc5 = glGetUniformLocation(sprite.shaderRef->second.GetHandle(), "TextureDimensions");
      GLint uniform_var_loc6 = glGetUniformLocation(sprite.shaderRef->second.GetHandle(), "LightTimer");
      GLint uniform_var_loc7 = glGetUniformLocation(sprite.shaderRef->second.GetHandle(), "ShakeTimer");
      GLint uniform_var_loc8 = glGetUniformLocation(sprite.shaderRef->second.GetHandle(), "ShakeScreen");
      GLuint tex_loc = glGetUniformLocation(sprite.shaderRef->second.GetHandle(), "uTex2d");

      if (uniform_var_loc1 >= 0)
      {
        glm::mat4 mModelNDC;

        float eyeRadius = 1.0f * oi.eyeRadius;
        float oneStep = 3.141590118f / 36;
        float eyeAlpha = oneStep * oi.eyeAlpha;
        float eyeBeta = oneStep * oi.eyeBeta;

        glm::vec3 view{ eyeRadius * cos(eyeAlpha) * cos(eyeBeta),
                        eyeRadius * sin(eyeAlpha),
                        eyeRadius * cos(eyeAlpha) * sin(eyeBeta) };

        glm::vec3 upVec{ 0.0f };

        if (oi.eyeAlpha == 18)
        {
          glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -oneStep * oi.eyeBeta, glm::vec3{ 0.0f, 1.0f, 1.0f });
          upVec = glm::mat3(rot) * -(glm::vec3{ 1.0f, 0.0f,0.0f });
        }
        else if (oi.eyeAlpha == -18)
        {
          glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -oneStep * oi.eyeBeta, glm::vec3{ 0.0f, 1.0f, 1.0f });
          upVec = glm::mat3(rot) * glm::vec3{ 1.0f, 0.0f,0.0f };
        }

        if (oi.eyeAlpha < 18)
        {
          upVec = glm::vec3{ 0.0f, 1.0f, 0.0f };
        }
        else
        {
          upVec = -(glm::vec3{ 0.0f, 1.0f, 0.0f });
        }

        glm::mat4 viewMtx = glm::lookAt(view, glm::vec3{ 0.0f, 0.0f, 0.0f }, upVec);
        glm::mat4 projMtx = glm::perspective(glm::radians(90.0f), static_cast<float>(GLHelper::width) / static_cast<float>(GLHelper::height), 1.0f, 500.0f);
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, { trans.x,trans.y,trans.z });
        model = glm::rotate(model, glm::radians(sphereRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(sphereRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(sphereRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, sphereScale);

        mModelNDC = projMtx * viewMtx * model;

        glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        //}
      }
      else
      {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
      }


      if (uniform_var_loc2 >= 0)
      {
        //float alpha = 1.0f;
        glUniform4f(uniform_var_loc2, sprite.color.x, sprite.color.y, sprite.color.z, sprite.transparency);
      }
      else
      {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
      }

      if (uniform_var_loc3 >= 0)
      {
        glUniform1i(uniform_var_loc3, sprite.hasTexture);
      }
      else
      {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
      }

      if (uniform_var_loc4 >= 0)
      {
        glUniform2f(uniform_var_loc4, sprite.textureIdx.x, sprite.textureIdx.y);
      }
      else
      {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
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
      else
      {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
      }

      if (uniform_var_loc8 >= 0)
      {
        glUniform1i(uniform_var_loc8, shakeScreen);
      }
      else
      {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
      }

      if (tex_loc >= 0)
      {
        glUniform1i(tex_loc, 1);
      }
      else
      {
        std::cout << "Uniform variable tex_loc doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
      }
    }
  }

}

void Eclipse::RenderSystem::DrawBuffers(unsigned int framebuffer)
{
  {
    //SortLayers();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    //for (auto& pair : lookup)
    for (auto const& pair : Graphics::sprites)
    {
      // Part 1: Install the shader program used by this object to render its model using GLSLShader::Use()
      pair.second->shaderRef->second.Use();

      // Part 2: Bind the object's VAO handle using glBindVertexArray
      glBindVertexArray(pair.second->modelRef->second->GetVaoID());

      glEnable(GL_BLEND);

      if (pair.second->modelRef == Graphics::FindModel("sphere") ||
        pair.second->modelRef == Graphics::FindModel("cube") ||
        pair.second->modelRef == Graphics::FindModel("cone") ||
        pair.second->modelRef == Graphics::FindModel("torus") ||
        pair.second->modelRef == Graphics::FindModel("pyramid") ||
        pair.second->modelRef == Graphics::FindModel("cylinder"))
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glEnable(GL_LINE_SMOOTH);
      }
      else
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_CULL_FACE);
      }

      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      if (pair.second->hasTexture)
      {
        if (pair.second->textureRef != Graphics::textures.end())
        {
          glBindTextureUnit(1, pair.second->textureRef->second.GetHandle());

          glEnable(GL_BLEND);

          glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
      }

      CheckUniformLoc(*(pair.second));

      // Part 4: Render OpenGL primitives encapsulated by this object's VAO using glDrawElements
      if (pair.second->modelRef->second->GetPrimitiveType() != GL_LINES)
      {
        glDrawElements(pair.second->modelRef->second->GetPrimitiveType(),
          pair.second->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL);
      }
      else
      {
        //glLineWidth() is actually a deprecated feature, so highly likely not to
        // work on a newer core version of OpenGL
        glLineWidth(5.0f);
        glDrawArrays(pair.second->modelRef->second->GetPrimitiveType(), 0, 2);
      }

      // Part 5: Clean up
      glBindVertexArray(0);
      pair.second->shaderRef->second.UnUse();
    }
  }
}

void Eclipse::RenderSystem::DrawSecondBuffers(unsigned int framebuffer)
{
  {
    //SortLayers();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    //for (auto& pair : lookup)
    for (auto const& pair : Graphics::sprites)
    {
      // Part 1: Install the shader program used by this object to render its model using GLSLShader::Use()
      pair.second->shaderRef->second.Use();

      // Part 2: Bind the object's VAO handle using glBindVertexArray
      glBindVertexArray(pair.second->modelRef->second->GetVaoID());

      glEnable(GL_BLEND);

      if (pair.second->modelRef == Graphics::FindModel("sphere") ||
        pair.second->modelRef == Graphics::FindModel("cube") ||
        pair.second->modelRef == Graphics::FindModel("cone") ||
        pair.second->modelRef == Graphics::FindModel("torus") ||
        pair.second->modelRef == Graphics::FindModel("pyramid") ||
        pair.second->modelRef == Graphics::FindModel("cylinder"))
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);
        glEnable(GL_LINE_SMOOTH);
      }
      else
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_CULL_FACE);
      }

      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      if (pair.second->hasTexture)
      {
        if (pair.second->textureRef != Graphics::textures.end())
        {
          glBindTextureUnit(1, pair.second->textureRef->second.GetHandle());

          glEnable(GL_BLEND);

          glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
      }

      CheckUniformLoc(*(pair.second));

      // Part 4: Render OpenGL primitives encapsulated by this object's VAO using glDrawElements
      if (pair.second->modelRef->second->GetPrimitiveType() != GL_LINES)
      {
        glDrawElements(pair.second->modelRef->second->GetPrimitiveType(),
          pair.second->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL);
      }
      else
      {
        //glLineWidth() is actually a deprecated feature, so highly likely not to
        // work on a newer core version of OpenGL
        glLineWidth(5.0f);
        glDrawArrays(pair.second->modelRef->second->GetPrimitiveType(), 0, 2);
      }

      // Part 5: Clean up
      glBindVertexArray(0);
      pair.second->shaderRef->second.UnUse();
    }
  }
}