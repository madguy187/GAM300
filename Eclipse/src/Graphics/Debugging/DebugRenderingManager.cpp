#include "pch.h"
#include "DebugRenderingManager.h"

void Eclipse::DebugRenderingManager::CheckUniformLoc(Graphics::shaderIt _shdrpgm, CameraComponent& _camera)
{
    GLint uniform_var_loc1 = _shdrpgm->second.GetLocation("uModelToNDC");
    GLint uniform_var_loc2 = _shdrpgm->second.GetLocation("uColor");
    GLint uniform_var_loc3 = _shdrpgm->second.GetLocation("uTextureCheck");
    //GLint uniform_var_loc4 = _shdrpgm->second.GetLocation("TextureIndex");
    //GLint uniform_var_loc5 = _shdrpgm->second.GetLocation("TextureDimensions");
    //GLint uniform_var_loc6 = _shdrpgm->second.GetLocation("LightTimer");
    //GLint uniform_var_loc7 = _shdrpgm->second.GetLocation("ShakeTimer");
    //GLint uniform_var_loc8 = _shdrpgm->second.GetLocation("ShakeScreen");
    GLuint tex_loc = _shdrpgm->second.GetLocation("uTex2d");

    if (uniform_var_loc1 >= 0)
    {
        glm::mat4 mModelNDC = _camera.projMtx * _camera.viewMtx * glm::mat4(1.0f);
        glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mModelNDC));
    }

    if (uniform_var_loc2 >= 0)
    {
        glUniform4f(uniform_var_loc2, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    if (uniform_var_loc3 >= 0)
    {
        glUniform1i(uniform_var_loc3, false);
    }

    //if (uniform_var_loc4 >= 0)
    //{
    //    glUniform2f(uniform_var_loc4, 0.0f, 0.0f);
    //}

    //if (uniform_var_loc7 >= 0)
    //{
    //    glUniform1f(uniform_var_loc7, 1.0f);
    //}

    //if (uniform_var_loc8 >= 0)
    //{
    //    glUniform1i(uniform_var_loc8, 1.0f);
    //}

    if (tex_loc >= 0)
    {
        glUniform1i(tex_loc, 1);
    }
}

void Eclipse::DebugRenderingManager::AddCameraFrustum(unsigned int ID, CameraComponent& _camera)
{
    Frustum newFrustum;
    newFrustum.UpdateFrustum(_camera);

    SetDebugShape(ID, newFrustum);
}

void Eclipse::DebugRenderingManager::DrawFrustum(unsigned int ID, unsigned int framebufferID)
{
    auto& lineArr = std::any_cast<Frustum>(debugShapes[ID]).GetLineSegments();
    auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

    auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    for (auto& it : lineArr)
    {
        shdrpgm->second.Use();

        glBindVertexArray(it.GetVaoID());

        glEnable(GL_BLEND);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glEnable(GL_LINE_SMOOTH);
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        CheckUniformLoc(shdrpgm, _camera);

        glLineWidth(5.0f); //Note that glLineWidth() is deprecated
        glDrawArrays(it.GetPrimitiveType(), 0, 2);

        glBindVertexArray(0);
        shdrpgm->second.UnUse();
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
