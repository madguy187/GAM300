#include "pch.h"

#include "Graphics/MeshEditor/MeshEditorSource.h"
#include "Editor/Windows/MeshEditor/MeshEditor.h"

namespace Eclipse
{
    MeshEditorSource::MeshEditorSource()
    {
        LightSource.Direction = ECVec3{ -10.0f,-10.0f,-10.0f };
    }

    void MeshEditorSource::Render()
    {
        auto* meshEditor = engine->editorManager->GetEditorWindow<MeshEditorWindow>();

        if (meshEditor->IsVisible)
        {
            Entity meshID = meshEditor->GetOldestParentID();
            auto& newMesh = engine->world.GetComponent<MeshComponent>(meshID);

            engine->MaterialManager.DoNotUpdateStencil();
            MeshEditorDraw(engine->world, newMesh, meshID,FrameBufferMode::FBM_MESHEDITOR, CameraComponent::CameraType::MeshEditor_Camera);

            if (engine->world.CheckComponent<ParentComponent>(meshID))
            {
                auto& parentCom = engine->world.GetComponent<ParentComponent>(meshID);
                
                for (const auto& kid : parentCom.child)
                {
                    auto& newKidMesh = engine->world.GetComponent<MeshComponent>(kid);

                    engine->MaterialManager.DoNotUpdateStencil();
                    MeshEditorDraw(engine->world, newKidMesh, kid, FrameBufferMode::FBM_MESHEDITOR, CameraComponent::CameraType::MeshEditor_Camera);
                }
            }
        }
    }

    void MeshEditorSource::MeshEditorDraw(World& world_, MeshComponent& ModelMesh, unsigned int ID, FrameBufferMode in, CameraComponent::CameraType _camType)
    {
        auto& _camera = world_.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(_camType));
        engine->gFrameBufferManager->UseFrameBuffer(in);

        auto shdrpgm = Graphics::shaderpgms["MeshEditor"];
        shdrpgm.Use();

        LightSourceUniform();
        engine->AssimpManager.MeshEditorUniforms(shdrpgm, _camera, ID);
        engine->AssimpManager.CheckUniforms(shdrpgm, ID, ModelMesh, _camera);
        engine->AssimpManager.RenderMesh(ModelMesh, GL_FILL);
    }

    void MeshEditorSource::LightSourceUniform()
    {
        auto shdrpgm = Graphics::shaderpgms["MeshEditor"];
        shdrpgm.Use();

        std::string number = std::to_string(0);
        GLint uniform_var_loc1 = shdrpgm.GetLocation(("directionlight[" + number + "].direction").c_str());
        GLint uniform_var_loc2 = shdrpgm.GetLocation(("directionlight[" + number + "].lightColor").c_str());

        GLCall(glUniform3f(uniform_var_loc1, LightSource.Direction.getX(), LightSource.Direction.getY(), LightSource.Direction.getZ()));
        GLCall(glUniform3f(uniform_var_loc2, 1500.0f, 1500.0f, 1500.0f));
    }
}