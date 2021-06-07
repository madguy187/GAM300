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
    GLuint uniform_var_loc10 = _shdrpgm->second.GetLocation("model");
    GLint uniform_var_loc11 = _shdrpgm->second.GetLocation("uTextureCheck");
    GLint uniform_var_loc12 = _shdrpgm->second.GetLocation(("pointLights[" + number + "].lightColor").c_str());
    GLint uniform_var_loc13 = _shdrpgm->second.GetLocation("uColor");

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
        glUniformMatrix4fv(uniform_var_loc10, 1, GL_FALSE, glm::value_ptr(model));
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

    if (uniform_var_loc11 >= 0)
    {
        glUniform1i(uniform_var_loc11, 0);
    }

    if (uniform_var_loc9 >= 0)
    {
        glUniform1i(uniform_var_loc9, 1);
    }

    // Light Color
    if (uniform_var_loc12 >= 0)
    {
        glUniform3f(uniform_var_loc12, 1.0f, 1.0f, 1.0f);
    }

    // Own Color
    if (uniform_var_loc13 >= 0)
    {
        glUniform3f(uniform_var_loc13, 1.0f, 0.99f, 0.5f);
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

    glLineWidth(5.0f); //Note that glLineWidth() is deprecated
    GLCall(glDrawElements(in->modelRef->second->GetPrimitiveType(), in->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));

    glBindVertexArray(0);
    in->shaderRef->second.UnUse();
}
