#pragma once

namespace Eclipse
{
    class MeshEditorSource
    {
    public:
        DirectionalLightComponent LightSource;
        Shader MeshEditorShader;

        MeshEditorSource();
        void Render();
        void MeshEditorDraw(World& world_, MeshComponent& ModelMesh, unsigned int ID, FrameBufferMode in, CameraComponent::CameraType _camType);
        void LightSourceUniform();
    };
}