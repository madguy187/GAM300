#include "pch.h"
#include "Graphics/Lighting/LightManager.h"

extern glm::vec3 spherepos;

void Eclipse::Lights::CreatePointLight()
{
    auto& GetWorld = engine->world;
    Entity EntityID = GetWorld.CreateEntity();

    GetWorld.AddComponent(EntityID, TransformComponent{});
    GetWorld.AddComponent(EntityID, PointLightComponent{});

    PointLightComponent& sprite = engine->world.GetComponent<PointLightComponent>(EntityID);
    sprite.ID = EntityID;
    sprite.shaderRef = Graphics::shaderpgms.find("nooblight");
    sprite.modelRef = Graphics::models.find("cube");

    _pointlights.insert({ sprite.ID, &sprite });
}

void Eclipse::Lights::DrawBuffers(unsigned int FrameBufferID, Eclipse::PointLightComponent* _spritecomponent, GLenum mode)
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID));
    _spritecomponent->shaderRef->second.Use();
    GLCall(glBindVertexArray(_spritecomponent->modelRef->second->GetVaoID()));

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CheckUniformLoc(*_spritecomponent, _spritecomponent->ID, FrameBufferID);

   GLCall(glDrawElements(_spritecomponent->modelRef->second->GetPrimitiveType(), _spritecomponent->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));

    // Part 5: Clean up
    glBindVertexArray(0);
    _spritecomponent->shaderRef->second.UnUse();
}

void Eclipse::Lights::CheckUniformLoc(PointLightComponent& sprite, unsigned int id, unsigned int framebufferID)
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

void Eclipse::Lights::DrawPointLights(unsigned int framebufferID)
{
    for (auto& it : _pointlights)
    {
        engine->gGraphics.ShowTestWidgets(it.first, (engine->gGraphics.createdID+1));
        DrawBuffers(framebufferID, it.second, GL_FILL);
    }
}

void Eclipse::Lights::SetMode(PointLightComponent& sprite)
{
    GLuint DirectionalLight = sprite.shaderRef->second.GetLocation("uTex2d");
    GLuint PointLight = sprite.shaderRef->second.GetLocation("uTex2d");
    GLuint Material = sprite.shaderRef->second.GetLocation("uTex2d");
}
