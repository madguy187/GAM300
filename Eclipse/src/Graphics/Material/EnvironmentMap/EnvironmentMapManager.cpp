#include "pch.h"
#include "Graphics/Material/EnvironmentMap/EnvironmentMapManager.h"

namespace Eclipse
{
    void EnvironmentMapManager::CheckUniform(MeshComponent& ModelMesh, CameraComponent& Camera)
    {
        TransformComponent camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(Camera.camType));

        if (engine->GraphicsManager.EnableEnvironmentMapForAll && engine->GraphicsManager.DrawSky)
        {
            ModelMesh.ENV_MAP = true;
        }
        else
        {
            ModelMesh.ENV_MAP = false;
        }

        if (ModelMesh.ENV_MAP == true && engine->GraphicsManager.DrawSky == true)
        {
            auto shdrpgm = Graphics::shaderpgms["EnvironmentMap"];
            shdrpgm.Use();
            GLuint view = shdrpgm.GetLocation("view");
            GLuint cameraPos = shdrpgm.GetLocation("cameraPos");
            GLint projection = shdrpgm.GetLocation("projection");
            GLint TypeOfMap = shdrpgm.GetLocation("Type");

            glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(Camera.projMtx));
            glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(Camera.viewMtx));
            GLCall(glUniform3f(cameraPos, camerapos.position.getX(), camerapos.position.getY(), camerapos.position.getZ()));
            glUniform1i(TypeOfMap, static_cast<GLint>(ModelMesh.ENV_TYPE));
        }
    }
}