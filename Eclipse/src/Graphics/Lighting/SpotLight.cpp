#include "pch.h"
#include "Graphics/Lighting/SpotLight.h"

using namespace Eclipse;

SpotLightContainer SpotLight::GetContainer()
{
    return _spotlights;
}

unsigned int SpotLight::GetNumberOfSpotLights()
{
    return _spotlights.size();
}

void SpotLight::CreateSpotLight(unsigned int CreatedID)
{
    // Add Components
    engine->world.AddComponent(CreatedID, SpotLightComponent{});

    // Assign
    SpotLightComponent& sprite = engine->world.GetComponent<SpotLightComponent>(CreatedID);
    sprite.ID = CreatedID;
    sprite.shaderRef = &(Graphics::shaderpgms.find("shader3DShdrpgm")->second);
    sprite.modelRef = Graphics::models.find("cube")->second.get();

    // Success
    _spotlights.insert({ counter,&sprite });
    ENGINE_CORE_INFO("SpotLight Created Successfully");
    counter++;
}

void SpotLight::DrawSpotLights(unsigned int framebufferID)
{
    for (auto& it : _spotlights)
    {
        Draw(it.second, framebufferID, it.first, GL_FILL);
    }
}

void SpotLight::Draw(SpotLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    in->shaderRef->Use();
    glBindVertexArray(in->modelRef->GetVaoID());

    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CheckUniformLoc(in->shaderRef, *in, indexID, _spotlights.size());

    GLCall(glDrawElements(in->modelRef->GetPrimitiveType(), in->modelRef->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));

    glBindVertexArray(0);
    in->shaderRef->UnUse();
}

void SpotLight::CheckUniformLoc(Shader* _shdrpgm, SpotLightComponent& in_spot, int index, unsigned int containersize)
{
    std::string number = std::to_string(index);

    GLint uniform_var_loc1 = _shdrpgm->GetLocation(("spotLights[" + number + "].position").c_str());
    GLint uniform_var_loc2 = _shdrpgm->GetLocation(("spotLights[" + number + "].ambient").c_str());
    GLint uniform_var_loc3 = _shdrpgm->GetLocation(("spotLights[" + number + "].diffuse").c_str());
    GLint uniform_var_loc4 = _shdrpgm->GetLocation(("spotLights[" + number + "].specular").c_str());
    GLint uniform_var_loc5 = _shdrpgm->GetLocation(("spotLights[" + number + "].constant").c_str());
    GLint uniform_var_loc6 = _shdrpgm->GetLocation(("spotLights[" + number + "].linear").c_str());
    GLint uniform_var_loc7 = _shdrpgm->GetLocation(("spotLights[" + number + "].quadratic").c_str());
    GLint uniform_var_loc8 = _shdrpgm->GetLocation("uModelToNDC");
    GLuint uniform_var_loc10 = _shdrpgm->GetLocation("model");
    GLint uniform_var_loc11 = _shdrpgm->GetLocation("uTextureCheck");
    GLint uniform_var_loc12 = _shdrpgm->GetLocation(("spotLights[" + number + "].lightColor").c_str());
    GLint uniform_var_loc13 = _shdrpgm->GetLocation("uColor");
    GLint uniform_var_loc14 = _shdrpgm->GetLocation(("spotLights[" + number + "].IntensityStrength").c_str());
    GLint uniform_var_loc15 = _shdrpgm->GetLocation(("spotLights[" + number + "].cutOff").c_str());
    GLint uniform_var_loc16 = _shdrpgm->GetLocation(("spotLights[" + number + "].outerCutOff").c_str());
    GLint uniform_var_loc17 = _shdrpgm->GetLocation(("spotLights[" + number + "].direction").c_str());
    GLint uniform_var_loc18 = _shdrpgm->GetLocation("NumberOfSpotLights");

    // SpotLight Position
    TransformComponent& SpotlightTransform = engine->world.GetComponent<TransformComponent>(in_spot.ID);

    // Which Camera's matrix
    CameraComponent& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

    if (uniform_var_loc8 >= 0)
    {
        glm::mat4 mModelNDC;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, SpotlightTransform.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(SpotlightTransform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(SpotlightTransform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(SpotlightTransform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, SpotlightTransform.scale.ConvertToGlmVec3Type());
        mModelNDC = camera.projMtx * camera.viewMtx * model;
        glUniformMatrix4fv(uniform_var_loc8, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        glUniformMatrix4fv(uniform_var_loc10, 1, GL_FALSE, glm::value_ptr(model));
    }

    // position
    if (uniform_var_loc1 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc1, SpotlightTransform.position.getX(), SpotlightTransform.position.getY(), SpotlightTransform.position.getZ()));
    }

    // ambient
    if (uniform_var_loc2 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc2, in_spot.ambient.x, in_spot.ambient.y, in_spot.ambient.z));
    }

    // diffuse
    if (uniform_var_loc3 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc3, in_spot.diffuse.x, in_spot.diffuse.y, in_spot.diffuse.z));
    }

    // specular
    if (uniform_var_loc4 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc4, in_spot.specular.x, in_spot.specular.y, in_spot.specular.z));
    }

    // constant
    if (uniform_var_loc5 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc5, in_spot.constant));
    }

    // linear
    if (uniform_var_loc6 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc6, in_spot.linear));
    }

    // quadratic
    if (uniform_var_loc7 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc7, in_spot.quadratic));
    }

    if (uniform_var_loc11 >= 0)
    {
        GLCall(glUniform1i(uniform_var_loc11, in_spot.hasTexture));
    }

    // Light Color
    if (uniform_var_loc12 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc12, in_spot.lightColor.x, in_spot.lightColor.y, in_spot.lightColor.z));
    }

    // Own Color
    if (uniform_var_loc13 >= 0)
    {
        GLCall(glUniform4f(uniform_var_loc13, in_spot.Color.x, in_spot.Color.y, in_spot.Color.z, in_spot.Color.w));
    }

    // Intensity of Light
    if (uniform_var_loc14 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc14, in_spot.IntensityStrength));
    }

    // cutOff
    if (uniform_var_loc15 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc15, glm::cos(glm::radians(12.5f))));
    }

    // outerCutOff
    if (uniform_var_loc16 >= 0)
    {
        GLCall(glUniform1f(uniform_var_loc16, glm::cos(glm::radians(27.5f))));
    }

    // position
    if (uniform_var_loc17 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc17, in_spot.direction.x, in_spot.direction.y, in_spot.direction.z));
    }

    if (uniform_var_loc18 >= 0)
    {
        GLCall(glUniform1i(uniform_var_loc18, containersize));
    }
}
