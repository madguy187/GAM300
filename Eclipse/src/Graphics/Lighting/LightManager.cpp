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

    _pointlights.push_back(&sprite);
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
    glDisable(GL_CULL_FACE);
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
        engine->gGraphics.ShowTestWidgets(it->ID, (engine->gGraphics.createdID+1));
        //DrawBuffers(framebufferID, it, GL_FILL);
        TESTDRAW(it->ID, framebufferID);
    }
}

void Eclipse::Lights::SetMode(PointLightComponent& sprite)
{
    GLuint DirectionalLight = sprite.shaderRef->second.GetLocation("uTex2d");
    GLuint PointLight = sprite.shaderRef->second.GetLocation("uTex2d");
    GLuint Material = sprite.shaderRef->second.GetLocation("uTex2d");
}


void Eclipse::Lights::CheckUniformLoc(Graphics::shaderIt _shdrpgm, PointLightComponent& hi)
{
    GLint uniform_var_loc1 = _shdrpgm->second.GetLocation("pointLights[0].position");
    GLint uniform_var_loc2 = _shdrpgm->second.GetLocation("pointLights[0].ambient");
    GLint uniform_var_loc3 = _shdrpgm->second.GetLocation("pointLights[0].diffuse");
    GLint uniform_var_loc4 = _shdrpgm->second.GetLocation("pointLights[0].specular");
    GLint uniform_var_loc5 = _shdrpgm->second.GetLocation("pointLights[0].constant");
    GLint uniform_var_loc6 = _shdrpgm->second.GetLocation("pointLights[0].linear");
    GLint uniform_var_loc7 = _shdrpgm->second.GetLocation("pointLights[0].quadratic");
    GLint uniform_var_loc8 = _shdrpgm->second.GetLocation("uModelToNDC");

    GLuint model2 = _shdrpgm->second.GetLocation("model");
    GLint check = _shdrpgm->second.GetLocation("uTextureCheck");

    TransformComponent& trans = engine->world.GetComponent<TransformComponent>(hi.ID);

    CameraComponent camera;
    camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

    if (uniform_var_loc8 >= 0)
    {
        glm::mat4 mModelNDC;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, trans.pos);
        model = glm::rotate(model, glm::radians(trans.rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, trans.scale);
        mModelNDC = camera.projMtx * camera.viewMtx * model;
        glUniformMatrix4fv(uniform_var_loc8, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(model));
    }

    if (uniform_var_loc1 >= 0)
    {
        glUniform3f(uniform_var_loc1, trans.pos.x, trans.pos.y, trans.pos.z);
    }

    if (uniform_var_loc2 >= 0)
    {
        glUniform3f(uniform_var_loc2, 0.05f, 0.05f, 0.05f);
    }

    if (uniform_var_loc3 >= 0)
    {
        glUniform3f(uniform_var_loc3, 0.8f, 0.8f, 0.8f);
    }

    if (uniform_var_loc4 >= 0)
    {
        glUniform3f(uniform_var_loc4, 1, 1, 1);
    }

    if (uniform_var_loc5 >= 0)
    {
        glUniform1i(uniform_var_loc5, 1.0f);
    }

    if (uniform_var_loc6 >= 0)
    {
        glUniform1i(uniform_var_loc6, 0.09f);
    }

    if (uniform_var_loc7 >= 0)
    {
        glUniform1i(uniform_var_loc7, 0.032);
    }

    if (check >= 0)
    {
        glUniform1i(check, false);
    }

}

void Eclipse::Lights::TESTDRAW(unsigned int ID, unsigned int framebufferID)
{
    auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

    auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    for (auto& it : _pointlights)
    {
        shdrpgm->second.Use();

        glBindVertexArray(it->modelRef->second->GetVaoID());

        glEnable(GL_BLEND);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);
        glEnable(GL_LINE_SMOOTH);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        CheckUniformLoc(shdrpgm, *it);

        glLineWidth(5.0f); //Note that glLineWidth() is deprecated
        GLCall(glDrawElements(it->modelRef->second->GetPrimitiveType(), it->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));

        glBindVertexArray(0);
        shdrpgm->second.UnUse();
    }
}