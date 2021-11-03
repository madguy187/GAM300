#include "pch.h"
#include "Graphics/Lighting/DirectionalLight.h"

namespace Eclipse
{
    void DirectionalLight::CreateDirectionalLight(unsigned int CreatedID)
    {
        // Check if already have Directional Light , If yes , Do not continue
        if (DirectionalLightcounter == 1)
        {
            EDITOR_LOG_WARN("Already Have Directional Light");
            return;
        }

        // Add DirectionalLight Component
        engine->world.AddComponent(CreatedID, LightComponent{});
        engine->world.AddComponent(CreatedID, DirectionalLightComponent{ DirectionalLightcounter });

        auto& Tr = engine->world.GetComponent<TransformComponent>(CreatedID);
        Tr.position = ECVec3(0.0f, 150.0f, 150.0f);

        EDITOR_LOG_INFO("Directional Light Created Successfully");
        DirectionalLightcounter++;
    }

    void DirectionalLight::Draw(unsigned int EntityId, DirectionalLightComponent* in, FrameBufferMode Mode, unsigned int IndexID, GLenum mode)
    {
        engine->gFrameBufferManager->UseFrameBuffer(Mode);

        //auto& shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];
        //shdrpgm.Use();
        //
        //glBindVertexArray(Graphics::models["Sphere"]->GetVaoID());
        //
        glEnable(GL_BLEND);
        glPolygonMode(GL_FRONT_AND_BACK, mode);
        glDisable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //
        //CheckUniformLoc(&shdrpgm, *in, IndexID, DirectionalLightcounter, EntityId);
        //
        //auto& Light = engine->world.GetComponent<LightComponent>(EntityId);

        //if (in->visible && Light.Render)
        //{
        //    GLCall(glDrawElements(Graphics::models["Sphere"]->GetPrimitiveType(),
        //        Graphics::models["Sphere"]->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));
        //}

        //glBindVertexArray(0);
        //shdrpgm.UnUse();

        CheckUniformPBR(IndexID, EntityId);
    }

    void DirectionalLight::CheckUniformPBR(int index, unsigned int EntityId)
    {
        auto shdrpgm = Graphics::shaderpgms["PBRShader"];
        shdrpgm.Use();

        DirectionalLightComponent& DLight = engine->world.GetComponent<DirectionalLightComponent>(EntityId);
        TransformComponent& DLightTrans = engine->world.GetComponent<TransformComponent>(EntityId);

        std::string number = std::to_string(index);
        GLint uniform_var_loc1 = shdrpgm.GetLocation(("directionlight[" + number + "].direction").c_str());
        GLint uniform_var_loc2 = shdrpgm.GetLocation(("directionlight[" + number + "].lightColor").c_str());
        GLint uniform_var_loc3 = shdrpgm.GetLocation(("directionlight[" + number + "].position").c_str());

        GLCall(glUniform3f(uniform_var_loc1, DLight.Direction.getX(), DLight.Direction.getY(), DLight.Direction.getZ()));
        GLCall(glUniform3f(uniform_var_loc3, DLightTrans.position.getX(), DLightTrans.position.getY(), DLightTrans.position.getZ()));
        GLCall(glUniform3f(uniform_var_loc2, 8.0f, 8.0f, 8.0f));

        shdrpgm.UnUse();
    }

    void DirectionalLight::CheckUniformLoc(Shader* _shdrpgm, DirectionalLightComponent& in_light, int index, unsigned int containersize, unsigned int EntityId)
    {
        (void)containersize;

        // We should only have 1 but lets see how
        std::string number = std::to_string(index);

        // First Check if it Affects World
        GLint uniform_var_loc11 = _shdrpgm->GetLocation(("directionlight[" + number + "].AffectsWorld").c_str());
        GLint uniform_var_loc5 = _shdrpgm->GetLocation("uModelToNDC");
        GLuint uniform_var_lo6 = _shdrpgm->GetLocation("model");

        TransformComponent& trans = engine->world.GetComponent<TransformComponent>(EntityId);
        CameraComponent camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

        // mModelNDC and model 
        glm::mat4 mModelNDC;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, trans.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(trans.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(trans.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, trans.scale.ConvertToGlmVec3Type());
        mModelNDC = camera.projMtx * camera.viewMtx * model;
        GLCall(glUniformMatrix4fv(uniform_var_loc5, 1, GL_FALSE, glm::value_ptr(mModelNDC)));
        GLCall(glUniformMatrix4fv(uniform_var_lo6, 1, GL_FALSE, glm::value_ptr(model)));
        GLCall(glUniform1i(uniform_var_loc11, in_light.AffectsWorld));

        if (in_light.AffectsWorld == true)
        {
            GLint uniform_var_loc1 = _shdrpgm->GetLocation(("directionlight[" + number + "].direction").c_str());
            GLint uniform_var_loc2 = _shdrpgm->GetLocation(("directionlight[" + number + "].ambient").c_str());
            GLint uniform_var_loc3 = _shdrpgm->GetLocation(("directionlight[" + number + "].diffuse").c_str());
            GLint uniform_var_loc4 = _shdrpgm->GetLocation(("directionlight[" + number + "].specular").c_str());
            GLint uniform_var_loc8 = _shdrpgm->GetLocation(("directionlight[" + number + "].lightColor").c_str());
            GLint uniform_var_loc10 = _shdrpgm->GetLocation("uColor");
            GLint useBlinn_ = _shdrpgm->GetLocation("useBlinn");

            // Direction
            GLCall(glUniform3f(uniform_var_loc1, in_light.Direction.getX(), in_light.Direction.getY(), in_light.Direction.getZ()));

            // ambient
            GLCall(glUniform3f(uniform_var_loc2, in_light.ambient.getX(), in_light.ambient.getY(), in_light.ambient.getZ()));

            // diffuse
            GLCall(glUniform3f(uniform_var_loc3, in_light.diffuse.getX(), in_light.diffuse.getY(), in_light.diffuse.getZ()));

            // specular
            GLCall(glUniform3f(uniform_var_loc4, in_light.specular.getX(), in_light.specular.getY(), in_light.specular.getZ()));

            // LightColor
            GLCall(glUniform3f(uniform_var_loc8, in_light.lightColor.getX(), in_light.lightColor.getY(), in_light.lightColor.getX()));

            // Own Color
            GLCall(glUniform4f(uniform_var_loc10, in_light.Color.x, in_light.Color.y, in_light.Color.z, in_light.Color.w));

            // Blinn phong
            GLCall(glUniform1i(useBlinn_, in_light.EnableBlinnPhong));
        }
    }

    void DirectionalLight::FirstGlobalLight()
    {
        auto FirstGlobalLight = engine->editorManager->CreateDefaultEntity(EntityType::ENT_LIGHT_DIRECTIONAL);

        // Add DirectionalLight Component
        engine->world.AddComponent(FirstGlobalLight, LightComponent{});
        engine->world.AddComponent(FirstGlobalLight, DirectionalLightComponent{ DirectionalLightcounter });

        auto& Tr = engine->world.GetComponent<TransformComponent>(FirstGlobalLight);
        Tr.position = ECVec3(0, 150, 150);

        DirectionalLightcounter++;
    }

    void DirectionalLight::Destroy()
    {
        --DirectionalLightcounter;
    }
}
namespace Eclipse
{
    bool DirectionalLight::InsertDirectionalLight(DirectionalLightComponent& in)
    {
        (void)in;

        //if (_DirectionalLight.insert({ in.ID , &in }).second == true)
        //{
        //	return true;
        //}

        return false;
    }

    void DirectionalLight::ClearContainer()
    {
        _DirectionalLight.clear();
    }

    DirectionalLightContainer DirectionalLight::GetContainer()
    {
        return _DirectionalLight;
    }

    bool DirectionalLight::DeleteDirectionalLight(unsigned int EntityID)
    {
        DLIT it = _DirectionalLight.find(EntityID);

        if (it == _DirectionalLight.end())
        {
            return false;
        }
        else
        {
            _DirectionalLight.erase(EntityID);
            --DirectionalLightcounter;

            EDITOR_LOG_INFO("DirectionalLight Removed Successfully");
            return true;
        }
    }
}