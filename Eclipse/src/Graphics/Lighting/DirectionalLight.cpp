#include "pch.h"
#include "Graphics/Lighting/DirectionalLight.h"

void Eclipse::DirectionalLight::CreateDirectionalLight(unsigned int CreatedID)
{
    std::cout << _DirectionalLight.size() << std::endl;

    // Add Components
    auto& GetWorld = engine->world;
    GetWorld.AddComponent(CreatedID, TransformComponent{});
    GetWorld.AddComponent(CreatedID, DirectionalLightComponent{});

    // Assign
    DirectionalLightComponent& _global = engine->world.GetComponent<DirectionalLightComponent>(CreatedID);
    _global.ID = CreatedID;
    _global.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");

    // Success
    _DirectionalLight.insert({ counter,&_global });
    ENGINE_CORE_INFO("DirectionalLight Created Successfully");
    counter++;
}

void Eclipse::DirectionalLight::Draw(unsigned int framebufferID)
{

}

void Eclipse::DirectionalLight::CheckUniformLoc(Graphics::shaderIt _shdrpgm, DirectionalLightComponent& hi, int index, unsigned int containersize)
{
    // We should only have 1 but lets see how
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

    // position
    if (uniform_var_loc1 >= 0)
    {
        glUniform3f(uniform_var_loc1, trans.pos.x, trans.pos.y, trans.pos.z);
    }

    // ambient
    if (uniform_var_loc2 >= 0)
    {
        glUniform3f(uniform_var_loc2, 0.2f, 0.2f, 0.2f);
    }

    // diffuse
    if (uniform_var_loc3 >= 0)
    {
        glUniform3f(uniform_var_loc3, 0.5f, 0.5f, 0.5f);
    }

    // specular
    if (uniform_var_loc4 >= 0)
    {
        glUniform3f(uniform_var_loc4, 0.1, 0.1, 0.1);
    }

    // constant
    if (uniform_var_loc5 >= 0)
    {
        glUniform1i(uniform_var_loc5, 1.0);
    }

    // linear
    if (uniform_var_loc6 >= 0)
    {
        glUniform1i(uniform_var_loc6, 0.09f);
    }

    if (uniform_var_loc7 >= 0)
    {
        glUniform1i(uniform_var_loc7, 0.032f);
    }

    if (check >= 0)
    {
        glUniform1i(check, false);
    }

    if (uniform_var_loc9 >= 0)
    {
        glUniform1i(uniform_var_loc5, 1);
    }
}

void Eclipse::DirectionalLight::Draw(DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID)
{
}
