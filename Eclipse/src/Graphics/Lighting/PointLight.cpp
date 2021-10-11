#include "pch.h"
#include "PointLight.h"

namespace Eclipse
{
    void PointLight::CreatePointLight(unsigned int CreatedID)
    {
        // Add PointLightComponent
        engine->world.AddComponent(CreatedID, LightComponent{});
        engine->world.AddComponent(CreatedID, PointLightComponent{ PointLightCounter });

        auto& AdjustSize = engine->world.GetComponent<TransformComponent>(CreatedID);
        AdjustSize.scale = ECVec3{ 0.1f,0.1f,0.1f };
        // PointLightComponent
        PointLightComponent& Light = engine->world.GetComponent<PointLightComponent>(CreatedID);
        engine->LightManager.SetAttenuation(Light, 5);

        // Insert into Container
        EDITOR_LOG_INFO("Pointlight Created Successfully");
        PointLightCounter++;
    }

    void PointLight::CheckUniformPBR(int index, unsigned int EntityId)
    {
        auto shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        TransformComponent& PointlightTransform = engine->world.GetComponent<TransformComponent>(EntityId);
        std::string number = std::to_string(index);
        GLint uniform_var_loc1 = shdrpgm.GetLocation(("pointLights[" + number + "].position").c_str());
        GLint uniform_var_loc2 = shdrpgm.GetLocation(("pointLights[" + number + "].lightColor").c_str());

        GLCall(glUniform3f(uniform_var_loc1, PointlightTransform.position.getX(), PointlightTransform.position.getY(), PointlightTransform.position.getZ()));
        GLCall(glUniform3f(uniform_var_loc2, 300.0f, 300.0f, 300.0f));

        shdrpgm.UnUse();
    }

    void PointLight::CheckUniformLoc(Shader* _shdrpgm, PointLightComponent& in_pointlight, int index, unsigned int containersize, unsigned int EntityId)
    {
        (void)containersize;

        GLint uniform_var_loc8 = _shdrpgm->GetLocation("uModelToNDC");
        //GLint uniform_var_loc9 = _shdrpgm->GetLocation("NumberOfPointLights");
        GLuint uniform_var_loc10 = _shdrpgm->GetLocation("model");

        // SpotLight Position
        TransformComponent& PointlightTransform = engine->world.GetComponent<TransformComponent>(EntityId);

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

        if (in_pointlight.AffectsWorld)
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
            GLint uniform_var_loc11 = _shdrpgm->GetLocation("uTextureCheck");
            GLint uniform_var_loc12 = _shdrpgm->GetLocation(("pointLights[" + number + "].lightColor").c_str());
            GLint uniform_var_loc13 = _shdrpgm->GetLocation("uColor");
            GLint uniform_var_loc14 = _shdrpgm->GetLocation(("pointLights[" + number + "].IntensityStrength").c_str());
            GLint useBlinn_ = _shdrpgm->GetLocation("useBlinn");


            // Position of Light
            GLCall(glUniform3f(uniform_var_loc1, PointlightTransform.position.getX(), PointlightTransform.position.getY(), PointlightTransform.position.getZ()));

            // ambient
            GLCall(glUniform3f(uniform_var_loc2, in_pointlight.ambient.getX(), in_pointlight.ambient.getY(), in_pointlight.ambient.getZ()));

            // diffuse
            GLCall(glUniform3f(uniform_var_loc3, in_pointlight.diffuse.getX(), in_pointlight.diffuse.getY(), in_pointlight.diffuse.getZ()));

            // specular
            GLCall(glUniform3f(uniform_var_loc4, in_pointlight.specular.getX(), in_pointlight.specular.getY(), in_pointlight.specular.getZ()));

            // constant
            GLCall(glUniform1f(uniform_var_loc5, in_pointlight.constant));

            // linear
            GLCall(glUniform1f(uniform_var_loc6, in_pointlight.linear));

            // quadratic
            GLCall(glUniform1f(uniform_var_loc7, in_pointlight.quadratic));

            // Check Texture
            GLCall(glUniform1i(uniform_var_loc11, in_pointlight.hasTexture));

            // Number Of PointLights
            //GLCall(glUniform1i(uniform_var_loc9, containersize));

            // Light Color
            GLCall(glUniform3f(uniform_var_loc12, in_pointlight.lightColor.getX(), in_pointlight.lightColor.getY(), in_pointlight.lightColor.getZ()));

            // Own Color
            GLCall(glUniform4f(uniform_var_loc13, in_pointlight.Color.getX(), in_pointlight.Color.getY(), in_pointlight.Color.getZ(), in_pointlight.Color.getW()));

            // Intensity of Light
            GLCall(glUniform1f(uniform_var_loc14, in_pointlight.IntensityStrength));

            // UseBlinn
            GLCall(glUniform1i(useBlinn_, in_pointlight.EnableBlinnPhong));
        }
    }

    void PointLight::Draw(unsigned int EntityId, PointLightComponent* in, FrameBufferMode Mode, unsigned int IndexID, GLenum mode)
    {
        engine->gFrameBufferManager->UseFrameBuffer(Mode);

        auto shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];
        shdrpgm.Use();

        glBindVertexArray(Graphics::models["Sphere"]->GetVaoID());

        glEnable(GL_BLEND);
        glPolygonMode(GL_FRONT_AND_BACK, mode);
        glDisable(GL_CULL_FACE);
        glEnable(GL_LINE_SMOOTH);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        CheckUniformLoc(&shdrpgm, *in, IndexID, PointLightCounter, EntityId);
        auto& Light = engine->world.GetComponent<LightComponent>(EntityId);

        if (in->visible && Light.Render)
        {
            GLCall(glDrawElements(Graphics::models["Sphere"]->GetPrimitiveType(),
                Graphics::models["Sphere"]->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));
        }

        glBindVertexArray(0);
        shdrpgm.UnUse();

        CheckUniformPBR(IndexID, EntityId);
    }

    void PointLight::Destroy()
    {
        --PointLightCounter;
    }

    unsigned int PointLight::GetNumberOfPointLights()
    {
        return PointLightCounter;
    }

}

namespace Eclipse
{
    void PointLight::ClearContainer()
    {
        _pointlights.clear();
    }

    bool PointLight::DeletePointLight(unsigned int EntityID)
    {
        PointL it = _pointlights.find(EntityID);

        if (it == _pointlights.end())
        {
            return false;
        }
        else
        {
            _pointlights.erase(EntityID);
            --PointLightCounter;

            EDITOR_LOG_INFO("Pointlight Removed Successfully");
            return true;
        }
    }

    PointLightContainer PointLight::GetContainer()
    {
        return _pointlights;
    }

    void PointLight::DrawPointLights(unsigned int framebufferID)
    {
        (void)framebufferID;

        for (auto& it : _pointlights)
        {
            Draw(it.first, it.second, FrameBufferMode::FBM_SCENE, it.first, GL_FILL);
        }
    }
}