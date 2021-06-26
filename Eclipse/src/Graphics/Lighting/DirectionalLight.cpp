#include "pch.h"
#include "Graphics/Lighting/DirectionalLight.h"

DirectionalLightContainer Eclipse::DirectionalLight::GetContainer()
{
    return _DirectionalLight;
}

void Eclipse::DirectionalLight::CreateDirectionalLight(unsigned int CreatedID)
{
    // Add Components
    auto& GetWorld = engine->world;
    GetWorld.AddComponent(CreatedID, DirectionalLightComponent{});

    // Assign
    DirectionalLightComponent& _global = engine->world.GetComponent<DirectionalLightComponent>(CreatedID);
    _global.ID = CreatedID;

    // Success
    _DirectionalLight.insert({ counter,&_global });
    ENGINE_CORE_INFO("DirectionalLight Created Successfully");
    counter++;
}

void Eclipse::DirectionalLight::Draw(DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");
    shdrpgm->second.Use();

    glBindVertexArray(Graphics::models.find("sphere")->second->GetVaoID());

    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CheckUniformLoc(shdrpgm, *in, indexID, _DirectionalLight.size());

    if (in->visible)
    {
        GLCall(glDrawElements(Graphics::models.find("sphere")->second->GetPrimitiveType(), Graphics::models.find("sphere")->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));
    }

    glBindVertexArray(0);
    shdrpgm->second.UnUse();
}

void Eclipse::DirectionalLight::CheckUniformLoc(Graphics::shaderIt _shdrpgm, DirectionalLightComponent& in_light, int index, unsigned int containersize)
{
    // We should only have 1 but lets see how
    std::string number = std::to_string(index);

    GLint uniform_var_loc1 = _shdrpgm->second.GetLocation(("directionlight[" + number + "].direction").c_str());
    GLint uniform_var_loc2 = _shdrpgm->second.GetLocation(("directionlight[" + number + "].ambient").c_str());
    GLint uniform_var_loc3 = _shdrpgm->second.GetLocation(("directionlight[" + number + "].diffuse").c_str());
    GLint uniform_var_loc4 = _shdrpgm->second.GetLocation(("directionlight[" + number + "].specular").c_str());
    GLint uniform_var_loc5 = _shdrpgm->second.GetLocation("uModelToNDC");
    GLuint uniform_var_lo6 = _shdrpgm->second.GetLocation("model");
    GLint uniform_var_loc8 = _shdrpgm->second.GetLocation(("directionlight[" + number + "].lightColor").c_str());
    GLint uniform_var_loc9 = _shdrpgm->second.GetLocation(("directionlight[" + number + "].visible").c_str());
    GLint uniform_var_loc10 = _shdrpgm->second.GetLocation("uColor");

    TransformComponent& trans = engine->world.GetComponent<TransformComponent>(in_light.ID);

    CameraComponent camera;
    camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

    if (uniform_var_loc5 >= 0 && uniform_var_lo6 >= 0)
    {
        glm::mat4 mModelNDC;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, trans.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(trans.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, trans.scale.ConvertToGlmVec3Type());
        mModelNDC = camera.projMtx * camera.viewMtx * model;
        glUniformMatrix4fv(uniform_var_loc5, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        glUniformMatrix4fv(uniform_var_lo6, 1, GL_FALSE, glm::value_ptr(model));
    }

    // Direction
    if (uniform_var_loc1 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc1, trans.position.getX() * -1, trans.position.getY() * -1, trans.position.getZ() * -1));
    }

    // ambient
    if (uniform_var_loc2 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc2, in_light.ambient.x, in_light.ambient.y, in_light.ambient.z));
    }

    // diffuse
    if (uniform_var_loc3 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc3, in_light.diffuse.x, in_light.diffuse.y, in_light.diffuse.z));
    }

    // specular
    if (uniform_var_loc4 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc4, in_light.specular.x, in_light.specular.y, in_light.specular.z));
    }

    // LightColor
    if (uniform_var_loc8 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc8, in_light.lightColor.x, in_light.lightColor.y, in_light.lightColor.z));
    }

    // Visibility
    if (uniform_var_loc9 >= 0)
    {
        GLCall(glUniform1i(uniform_var_loc9, in_light.visible));
    }

    // Own Color
    if (uniform_var_loc10 >= 0)
    {
        GLCall(glUniform4f(uniform_var_loc10, in_light.Color.x, in_light.Color.y, in_light.Color.z, in_light.Color.w));
    }

}

void Eclipse::DirectionalLight::FirstGlobalLight()
{
    // Add Components
    auto& GetWorld = engine->world;
    Entity CreatedID = GetWorld.CreateEntity();
    engine->world.AddComponent(CreatedID, DirectionalLightComponent{});

    // Assign
    DirectionalLightComponent& _global = engine->world.GetComponent<DirectionalLightComponent>(CreatedID);
    _global.ID = CreatedID;

    // Success
    _DirectionalLight.insert({ counter,&_global });
    ENGINE_CORE_INFO("First DirectionalLight Created Successfully");
    counter++;
}