#include "pch.h"
#include "PointLight.h"

PointLightContainer Eclipse::PointLight::GetContainer()
{
    return _pointlights;
}

unsigned int Eclipse::PointLight::GetNumberOfPointLights()
{
    return _pointlights.size();
}

void Eclipse::PointLight::CreatePointLight(unsigned int CreatedID)
{
    // Add Components
    auto& GetWorld = engine->world;
    GetWorld.AddComponent(CreatedID, PointLightComponent{});

    // Assign
    PointLightComponent& sprite = engine->world.GetComponent<PointLightComponent>(CreatedID);
    sprite.ID = CreatedID;
    sprite.shaderRef = &(Graphics::shaderpgms.find("shader3DShdrpgm")->second);
    sprite.modelRef = Graphics::models.find("sphere")->second.get();
    engine->LightManager.SetAttenuation(sprite, 5);

    TransformComponent& transform = engine->world.GetComponent<TransformComponent>(CreatedID);
    transform.scale.setX(1.0f);
    transform.scale.setY(1.0f);
    transform.scale.setZ(1.0f);

    // Success
    _pointlights.insert({ counter,&sprite });
    EDITOR_LOG_INFO("Pointlight Created Successfully");
    counter++;
}

void Eclipse::PointLight::DrawPointLights(unsigned int framebufferID)
{
    for (auto& it : _pointlights)
    {
        Draw(it.second, framebufferID, it.first, GL_FILL);
    }
}

void Eclipse::PointLight::CheckUniformLoc(Shader* _shdrpgm, PointLightComponent& in_pointlight, int index, unsigned int containersize)
{
    // Custom Variables into Shaders
    std::string number = std::to_string(index);
    GLint uniform_var_loc1 = _shdrpgm->GetLocation(("pointLights[" + number + "].position").c_str());
    GLint uniform_var_loc2 = _shdrpgm->GetLocation(("pointLights[" + number + "].ambient").c_str());
    GLint uniform_var_loc3 = _shdrpgm->GetLocation(("pointLights[" + number + "].diffuse").c_str());
    GLint uniform_var_loc4 = _shdrpgm->GetLocation(("pointLights[" + number + "].specular").c_str());
    GLint uniform_var_loc5 = _shdrpgm->GetLocation(("pointLights[" + number + "].constant").c_str());
    GLint uniform_var_loc6 = _shdrpgm->GetLocation(("pointLights[" + number + "].linear").c_str());
    GLint uniform_var_loc7 = _shdrpgm->GetLocation(("pointLights[" + number + "].quadratic").c_str());
    GLint uniform_var_loc8 = _shdrpgm->GetLocation("uModelToNDC");
    GLint uniform_var_loc9 = _shdrpgm->GetLocation("NumberOfPointLights");
    GLuint uniform_var_loc10 = _shdrpgm->GetLocation("model");
    GLint uniform_var_loc11 = _shdrpgm->GetLocation("uTextureCheck");
    GLint uniform_var_loc12 = _shdrpgm->GetLocation(("pointLights[" + number + "].lightColor").c_str());
    GLint uniform_var_loc13 = _shdrpgm->GetLocation("uColor");
    GLint uniform_var_loc14 = _shdrpgm->GetLocation(("pointLights[" + number + "].IntensityStrength").c_str());
    GLint useBlinn_ = _shdrpgm->GetLocation("useBlinn");

    // SpotLight Position
    TransformComponent& PointlightTransform = engine->world.GetComponent<TransformComponent>(in_pointlight.ID);

    // Which Camera's matrix
    CameraComponent& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

    // Model Matrix
    if (uniform_var_loc8 >= 0)
    {
        glm::mat4 mModelNDC;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, PointlightTransform.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(PointlightTransform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(PointlightTransform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(PointlightTransform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, PointlightTransform.scale.ConvertToGlmVec3Type());
        mModelNDC = camera.projMtx * camera.viewMtx * model;
        glUniformMatrix4fv(uniform_var_loc8, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        glUniformMatrix4fv(uniform_var_loc10, 1, GL_FALSE, glm::value_ptr(model));
    }

    // Position of Light
    if (uniform_var_loc1 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc1, PointlightTransform.position.getX(), PointlightTransform.position.getY(), PointlightTransform.position.getZ()));
    }

    // ambient
    if (uniform_var_loc2 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc2, in_pointlight.ambient.getX(), in_pointlight.ambient.getY(), in_pointlight.ambient.getZ()));
    }

    // diffuse
    if (uniform_var_loc3 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc3, in_pointlight.diffuse.getX(), in_pointlight.diffuse.getY(), in_pointlight.diffuse.getZ()));
    }

    // specular
    if (uniform_var_loc4 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc4, in_pointlight.specular.getX(), in_pointlight.specular.getY(), in_pointlight.specular.getZ()));
    }

    // constant
    if (uniform_var_loc5 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc5, in_pointlight.constant));
    }

    // linear
    if (uniform_var_loc6 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc6, in_pointlight.linear));
    }

    // quadratic
    if (uniform_var_loc7 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc7, in_pointlight.quadratic));
    }

    // Check Texture
    if (uniform_var_loc11 >= 0)
    {
        GLCall(glUniform1i(uniform_var_loc11, in_pointlight.hasTexture));
    }

    // Number Of PointLights
    if (uniform_var_loc9 >= 0)
    {
        GLCall(glUniform1i(uniform_var_loc9, containersize));
    }

    // Light Color
    if (uniform_var_loc12 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc12, in_pointlight.lightColor.getX(), in_pointlight.lightColor.getY(), in_pointlight.lightColor.getZ()));
    }

    // Own Color
    if (uniform_var_loc13 >= 0)
    {
        GLCall(glUniform4f(uniform_var_loc13, in_pointlight.Color.getX(), in_pointlight.Color.getY(), in_pointlight.Color.getZ(), in_pointlight.Color.getW()));
    }

    // Intensity of Light
    if (uniform_var_loc14 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc14, in_pointlight.IntensityStrength));
    }

    if (useBlinn_ >= 0)
    {
        GLCall(glUniform1i(useBlinn_, true));
    }
}

void Eclipse::PointLight::Draw(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    in->shaderRef->Use();
    glBindVertexArray(in->modelRef->GetVaoID());

    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glDisable(GL_CULL_FACE);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CheckUniformLoc(in->shaderRef, *in, indexID, _pointlights.size());

    if (in->visible)
    {
        GLCall(glDrawElements(in->modelRef->GetPrimitiveType(), in->modelRef->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));
    }

    glBindVertexArray(0);
    in->shaderRef->UnUse();
}
