#include "pch.h"
#include "Graphics/Lighting/DirectionalLight.h"

DirectionalLightContainer Eclipse::DirectionalLight::GetContainer()
{
    return _DirectionalLight;
}

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
    _global.modelRef = Graphics::models.find("cube");

    // Success
    _DirectionalLight.insert({ counter,&_global });
    ENGINE_CORE_INFO("DirectionalLight Created Successfully");
    counter++;
}

void Eclipse::DirectionalLight::Draw(DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    in->shaderRef->second.Use();
    glBindVertexArray(in->modelRef->second->GetVaoID());

    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CheckUniformLoc(in->shaderRef, *in, indexID, _DirectionalLight.size());

    GLCall(glDrawElements(in->modelRef->second->GetPrimitiveType(), in->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));

    glBindVertexArray(0);
    in->shaderRef->second.UnUse();
}

void Eclipse::DirectionalLight::CheckUniformLoc(Graphics::shaderIt _shdrpgm, DirectionalLightComponent& hi, int index, unsigned int containersize)
{
    // We should only have 1 but lets see how
    std::string number = std::to_string(index);

    GLint uniform_var_loc1 = _shdrpgm->second.GetLocation(("directionlight[" + number + "].direction").c_str());
    GLint uniform_var_loc2 = _shdrpgm->second.GetLocation(("directionlight[" + number + "].ambient").c_str());
    GLint uniform_var_loc3 = _shdrpgm->second.GetLocation(("directionlight[" + number + "].diffuse").c_str());
    GLint uniform_var_loc4 = _shdrpgm->second.GetLocation(("directionlight[" + number + "].specular").c_str());
    GLint uniform_var_loc5 = _shdrpgm->second.GetLocation("uModelToNDC");
    GLuint uniform_var_lo6 = _shdrpgm->second.GetLocation("model");
    GLint uniform_var_loc7 = _shdrpgm->second.GetLocation("uTextureCheck");

    TransformComponent& trans = engine->world.GetComponent<TransformComponent>(hi.ID);

    CameraComponent camera;
    camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

    // Direction
    if (uniform_var_loc1 >= 0)
    {
        //GLCall(glUniform3f(uniform_var_loc1, trans.pos.x * -1, trans.pos.y * -1, trans.pos.z * -1));
    }

    // ambient
    if (uniform_var_loc2 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc2, 0.05f, 0.05f, 0.05f));
    }

    // diffuse
    if (uniform_var_loc3 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc3, 0.4f, 0.4f, 0.4f));
    }

    // specular
    if (uniform_var_loc4 >= 0)
    {
        GLCall(glUniform3f(uniform_var_loc4, 0.5f, 0.5f, 0.5f));
    }

    if (uniform_var_loc5 >= 0 && uniform_var_lo6 >= 0)
    {
        glm::mat4 mModelNDC;

        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, trans.pos);
        //model = glm::rotate(model, glm::radians(trans.rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(trans.rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(trans.rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        //model = glm::scale(model, trans.scale);
        //mModelNDC = camera.projMtx * camera.viewMtx * model;
        //GLCall(glUniformMatrix4fv(uniform_var_loc5, 1, GL_FALSE, glm::value_ptr(mModelNDC)));
        //GLCall(glUniformMatrix4fv(uniform_var_lo6, 1, GL_FALSE, glm::value_ptr(model)));

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

    if (uniform_var_loc7 >= 0)
    {
        GLCall(glUniform1i(uniform_var_loc7, 0));
    }
}

void Eclipse::DirectionalLight::FirstGlobalLight()
{
    // Add Components
    auto& GetWorld = engine->world;
    Entity CreatedID = GetWorld.CreateEntity();
    GetWorld.AddComponent(CreatedID, TransformComponent{});
    GetWorld.AddComponent(CreatedID, DirectionalLightComponent{});

    // Assign
    DirectionalLightComponent& _global = engine->world.GetComponent<DirectionalLightComponent>(CreatedID);
    _global.ID = CreatedID;
    _global.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
    _global.modelRef = Graphics::models.find("cube");

    // Success
    _DirectionalLight.insert({ counter,&_global });
    ENGINE_CORE_INFO("DirectionalLight Created Successfully");
    counter++;
}