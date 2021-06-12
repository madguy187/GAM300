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
    std::cout << _pointlights.size() << std::endl;

    // Add Components
    auto& GetWorld = engine->world;
    GetWorld.AddComponent(CreatedID, TransformComponent{});
    GetWorld.AddComponent(CreatedID, PointLightComponent{});

    // Assign
    PointLightComponent& sprite = engine->world.GetComponent<PointLightComponent>(CreatedID);
    sprite.ID = CreatedID;
    sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
    sprite.modelRef = Graphics::models.find("cube");

    // Success
    _pointlights.insert({ counter,&sprite });
    ENGINE_CORE_INFO("Pointlight Created Successfully");
    counter++;
}

void Eclipse::PointLight::DrawPointLights(unsigned int framebufferID)
{
    for (auto& it : _pointlights)
    {
        engine->gGraphics.ShowTestWidgets(it.second->ID, (engine->gGraphics.createdID));
        Draw(it.second, framebufferID, it.first, GL_FILL);
    }
}

void Eclipse::PointLight::CheckUniformLoc(Graphics::shaderIt _shdrpgm, PointLightComponent& in_pointlight, int index, unsigned int containersize)
{
    std::string number = std::to_string(index);

    GLint uniform_var_loc1 = _shdrpgm->second.GetLocation(("pointLights[" + number + "].position").c_str());
    GLint uniform_var_loc2 = _shdrpgm->second.GetLocation(("pointLights[" + number + "].ambient").c_str());
    GLint uniform_var_loc3 = _shdrpgm->second.GetLocation(("pointLights[" + number + "].diffuse").c_str());
    GLint uniform_var_loc4 = _shdrpgm->second.GetLocation(("pointLights[" + number + "].specular").c_str());
    GLint uniform_var_loc5 = _shdrpgm->second.GetLocation(("pointLights[" + number + "].constant").c_str());
    GLint uniform_var_loc6 = _shdrpgm->second.GetLocation(("pointLights[" + number + "].linear").c_str());
    GLint uniform_var_loc7 = _shdrpgm->second.GetLocation(("pointLights[" + number + "].quadratic").c_str());
    GLint uniform_var_loc8 = _shdrpgm->second.GetLocation("uModelToNDC");
    GLint uniform_var_loc9 = _shdrpgm->second.GetLocation("NumberOfPointLights");
    GLuint uniform_var_loc10 = _shdrpgm->second.GetLocation("model");
    GLint uniform_var_loc11 = _shdrpgm->second.GetLocation("uTextureCheck");
    GLint uniform_var_loc12 = _shdrpgm->second.GetLocation(("pointLights[" + number + "].lightColor").c_str());
    GLint uniform_var_loc13 = _shdrpgm->second.GetLocation("uColor");
    GLint uniform_var_loc14 = _shdrpgm->second.GetLocation(("pointLights[" + number + "].IntensityStrength").c_str());

    TransformComponent& PointlightTransform = engine->world.GetComponent<TransformComponent>(in_pointlight.ID);
    CameraComponent& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

    if (uniform_var_loc8 >= 0)
    {
        glm::mat4 mModelNDC;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, PointlightTransform.pos);
        model = glm::rotate(model, glm::radians(PointlightTransform.rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(PointlightTransform.rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(PointlightTransform.rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, PointlightTransform.scale);
        mModelNDC = camera.projMtx * camera.viewMtx * model;
        GLCall(glUniformMatrix4fv(uniform_var_loc8, 1, GL_FALSE, glm::value_ptr(mModelNDC)));
        GLCall(glUniformMatrix4fv(uniform_var_loc10, 1, GL_FALSE, glm::value_ptr(model)));
    }

    // position
    if (uniform_var_loc1 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc1, PointlightTransform.pos.x, PointlightTransform.pos.y, PointlightTransform.pos.z));
    }

    // ambient
    if (uniform_var_loc2 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc2, in_pointlight.ambient.x, in_pointlight.ambient.y, in_pointlight.ambient.z));
    }

    // diffuse
    if (uniform_var_loc3 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc3, in_pointlight.diffuse.x, in_pointlight.diffuse.y, in_pointlight.diffuse.z));
    }

    // specular
    if (uniform_var_loc4 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc4, in_pointlight.specular.x, in_pointlight.specular.y, in_pointlight.specular.z));
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

    if (uniform_var_loc11 >= 0)
    {
        GLCall(glUniform1i(uniform_var_loc11, in_pointlight.hasTexture));
    }

    if (uniform_var_loc9 >= 0)
    {
        GLCall(glUniform1i(uniform_var_loc9, containersize));
    }

    // Light Color
    if (uniform_var_loc12 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc12, in_pointlight.lightColor.x, in_pointlight.lightColor.y, in_pointlight.lightColor.z));
    }

    // Own Color
    if (uniform_var_loc13 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc13, in_pointlight.Color.x, in_pointlight.Color.y, in_pointlight.Color.z));
    }

    // Intensity of Light
    if (uniform_var_loc14 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc14, in_pointlight.IntensityStrength));
    }
}

void Eclipse::PointLight::Draw(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    in->shaderRef->second.Use();
    glBindVertexArray(in->modelRef->second->GetVaoID());

    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glDisable(GL_CULL_FACE);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CheckUniformLoc(in->shaderRef, *in, indexID, _pointlights.size());

    GLCall(glDrawElements(in->modelRef->second->GetPrimitiveType(), in->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));

    glBindVertexArray(0);
    in->shaderRef->second.UnUse();
}
