#include "pch.h"
#include "Graphics/Lighting/SpotLight.h"

namespace Eclipse
{
	unsigned int SpotLight::GetNumberOfSpotLights()
	{
		return SpotLightCounter;
	}

	void SpotLight::CreateSpotLight(unsigned int CreatedID)
	{
		// Add SpotLightComponent
		engine->world.AddComponent(CreatedID, LightComponent{});
		engine->world.AddComponent(CreatedID, SpotLightComponent{ CreatedID , SpotLightCounter });

		// SpotLightComponent
		SpotLightComponent& SpotLight = engine->world.GetComponent<SpotLightComponent>(CreatedID);
		engine->LightManager.SetAttenuation(SpotLight, 5);

		// TransformComponent
		TransformComponent& transform = engine->world.GetComponent<TransformComponent>(CreatedID);
		transform.scale = ECVec3{ 0.1f,0.1f,0.1f };

		ENGINE_CORE_INFO("SpotLight Created Successfully");
		SpotLightCounter++;
	}

	void SpotLight::Draw(unsigned int EntityId,SpotLightComponent* in, unsigned int framebufferID, unsigned int IndexId, GLenum mode)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

		auto shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];
		shdrpgm.Use();

		glBindVertexArray(Graphics::models["Sphere"]->GetVaoID());

		glEnable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, mode);
		glDisable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		CheckUniformLoc(&shdrpgm, *in, IndexId, SpotLightCounter);

		auto& Light = engine->world.GetComponent<LightComponent>(EntityId);

		if (in->visible && Light.Render)
		{
			GLCall(glDrawElements(Graphics::models["Sphere"]->GetPrimitiveType(),
				Graphics::models["Sphere"]->GetDrawCount(), GL_UNSIGNED_SHORT, NULL));
		}

		glBindVertexArray(0);
		shdrpgm.UnUse();
	}

	void SpotLight::CheckUniformLoc(Shader* _shdrpgm, SpotLightComponent& in_spot, int index, unsigned int containersize)
	{
		GLint uniform_var_loc8 = _shdrpgm->GetLocation("uModelToNDC");
		GLuint uniform_var_loc10 = _shdrpgm->GetLocation("model");

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

		if (in_spot.AffectsWorld)
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
			GLint uniform_var_loc19 = _shdrpgm->GetLocation(("spotLights[" + number + "].SurroundingAttenuationLevel").c_str());
			GLint useBlinn_ = _shdrpgm->GetLocation("useBlinn");

			// position
			GLCall(glUniform3f(uniform_var_loc1, SpotlightTransform.position.getX(), SpotlightTransform.position.getY(), SpotlightTransform.position.getZ()));

			// ambient
			GLCall(glUniform3f(uniform_var_loc2, in_spot.ambient.getX(), in_spot.ambient.getY(), in_spot.ambient.getZ()));

			// diffuse
			GLCall(glUniform3f(uniform_var_loc3, in_spot.diffuse.getX(), in_spot.diffuse.getY(), in_spot.diffuse.getZ()));

			// specular
			GLCall(glUniform3f(uniform_var_loc4, in_spot.specular.getX(), in_spot.specular.getY(), in_spot.specular.getZ()));

			// constant
			GLCall(glUniform1f(uniform_var_loc5, in_spot.constant));

			// linear
			GLCall(glUniform1f(uniform_var_loc6, in_spot.linear));

			// quadratic
			GLCall(glUniform1f(uniform_var_loc7, in_spot.quadratic));

			GLCall(glUniform1i(uniform_var_loc11, in_spot.hasTexture));

			// Light Color
			GLCall(glUniform3f(uniform_var_loc12, in_spot.lightColor.getX(), in_spot.lightColor.getY(), in_spot.lightColor.getZ()));

			// Own Color
			GLCall(glUniform4f(uniform_var_loc13, in_spot.Color.x, in_spot.Color.y, in_spot.Color.z, in_spot.Color.w));

			// Intensity of Light{
			GLCall(glUniform1f(uniform_var_loc14, in_spot.IntensityStrength));

			// cutOff
			GLCall(glUniform1f(uniform_var_loc15, glm::cos(glm::radians(in_spot.cutOff))));

			// outerCutOff
			GLCall(glUniform1f(uniform_var_loc16, glm::cos(glm::radians(in_spot.outerCutOff))));

			// position
			GLCall(glUniform3f(uniform_var_loc17, in_spot.direction.getX(), in_spot.direction.getY(), in_spot.direction.getZ()));

			GLCall(glUniform1i(uniform_var_loc18, containersize));

			GLCall(glUniform1f(uniform_var_loc19, in_spot.SurroundingAttenuationLevel));

			GLCall(glUniform1i(useBlinn_, in_spot.EnableBlinnPhong));
		}
	}
}

namespace Eclipse
{
	SpotLightContainer SpotLight::GetContainer()
	{
		return _spotlights;
	}

	bool SpotLight::InsertSpotLightLight(SpotLightComponent& in)
	{
		if (_spotlights.insert({ in.ID , &in }).second == true)
		{
			return true;
		}

		return false;
	}

	void SpotLight::ClearContainer()
	{
		_spotlights.clear();
	}

	bool SpotLight::DeleteSpotLight(unsigned int EntityID)
	{
		SLIT it = _spotlights.find(EntityID);

		if (it == _spotlights.end())
		{
			return false;
		}
		else
		{
			_spotlights.erase(EntityID);
			--SpotLightCounter;

			EDITOR_LOG_INFO("SpotLight Removed Successfully");
			return true;
		}
	}

	void SpotLight::DrawSpotLights(unsigned int framebufferID)
	{
		for (auto& it : _spotlights)
		{
			Draw(it.first,it.second, framebufferID, it.first, GL_FILL);
		}
	}

}