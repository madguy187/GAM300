#include "pch.h"
#include "DebugRenderingManager.h"

void Eclipse::DebugRenderingManager::CheckUniformLoc(Shader*_shdrpgm, CameraComponent& _camera)
{
    GLint uniform_var_loc1 = _shdrpgm->GetLocation("uModelToNDC");
    GLint uniform_var_loc2 = _shdrpgm->GetLocation("uColor");
    
    if (uniform_var_loc1 >= 0)
    {
        glm::mat4 mModelNDC = _camera.projMtx * _camera.viewMtx * glm::mat4(1.0f);
        glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mModelNDC));
    }

    if (uniform_var_loc2 >= 0)
    {
        glUniform4f(uniform_var_loc2, 0.0f, 0.0f, 0.0f, 1.0f);
    }
}

void Eclipse::DebugRenderingManager::AddCameraFrustum(unsigned int ID)
{
    Frustum newFrustum;
    SetDebugShape(ID, newFrustum);
}

void Eclipse::DebugRenderingManager::DrawFrustum(unsigned int ID, unsigned int framebufferID)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    auto& frustum = std::any_cast<Frustum>(debugShapes[ID]);
    frustum.UpdateFrustum(engine->world.GetComponent<CameraComponent>(ID));
    SetDebugShape(ID, frustum);

    auto& lineArr = std::any_cast<Frustum>(debugShapes[ID]).GetLineSegments();
    auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

    auto& shdrpgm = Graphics::shaderpgms["FrustrumShader"];

    for (unsigned int i = 0; i < lineArr.size(); ++i)
    {
        shdrpgm.Use();

        glBindVertexArray(frustum.GetVaoID());

        glNamedBufferSubData(frustum.GetVboID(), 0, 2 * sizeof(glm::vec3), &lineArr[i]);

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glEnable(GL_LINE_SMOOTH);
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        CheckUniformLoc(&shdrpgm, _camera);

        glLineWidth(5.0f); //Note that glLineWidth() is deprecated
        glDrawArrays(frustum.GetPrimitiveType(), 0, 2);

        glBindVertexArray(0);
        shdrpgm.UnUse();
    }
}

void Eclipse::DebugRenderingManager::SetDebugShape(unsigned int ID, std::any newShape)
{
    debugShapes[ID] = newShape;
}

void Eclipse::DebugRenderingManager::DrawDebugShapes(unsigned int framebufferID)
{
    for (auto& it : debugShapes)
    {
        if (it.second.type() == typeid(Frustum))
        {
            DrawFrustum(it.first, framebufferID);
        }
    }
}

void Eclipse::DebugRenderingManager::DeleteDebugShape(unsigned int ID)
{
    debugShapes.erase(ID);
}

void Eclipse::DebugRenderingManager::ClearDebugShapes()
{
    debugShapes.clear();
}
