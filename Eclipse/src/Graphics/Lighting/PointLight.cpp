#include "pch.h"
#include "PointLight.h"

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
        Draw(it.second, framebufferID, it.first);
    }
}

void Eclipse::PointLight::CheckUniformLoc(Graphics::shaderIt _shdrpgm, PointLightComponent& hi, int index, unsigned int containersize)
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

    if (uniform_var_loc9 >= 0)
    {
        glUniform1i(uniform_var_loc5, containersize);
    }
}

void Eclipse::PointLight::Draw(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    in->shaderRef->second.Use();

    glBindVertexArray(in->modelRef->second->GetVaoID());

    glEnable(GL_BLEND);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CheckUniformLoc(in->shaderRef, *in, indexID, _pointlights.size());

    glLineWidth(5.0f); //Note that glLineWidth() is deprecated
    GLCall(glDrawElements(in->modelRef->second->GetPrimitiveType(), in->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));

    glBindVertexArray(0);
    in->shaderRef->second.UnUse();
}
