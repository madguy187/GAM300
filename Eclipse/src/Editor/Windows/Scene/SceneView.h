#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
    struct SnapValueSettings
    {
        float mPosSnapValue{ 5.f };
        float mScaleSnapValue{ 1.f };
        float mRotSnapValue{ 45.f };
    };

    class SceneWindow final : public ECGuiWindow
    {
    public:
        void Update() override;
        void Init() override;
        void Unload() override;

        void RunMainWindow();
        void RunFrameBuffer();
        void OnGizmoUpdateEvent();
        void OnCameraMoveEvent();
        void OnCameraZoomEvent();
        void OnSelectEntityEvent();

        SnapValueSettings& GetRefToSnapSettings();
        SnapValueSettings GetSnapSettings();
        glm::vec2 GetSceneBufferPos();
        glm::vec2 GetSceneBufferSize();
        glm::vec2 GetCursorScreenPos();
        int GetGizmoType() const;
        bool GetIsWindowActive() const;
        bool GetSnapping() const;

        void SetGizmoType(int type);
        void SetSnapping(bool active);
    private:
        ECVec2 mViewportSize;
        ECVec2 mSceneBufferSize;
        ECVec2 mSceneBufferPos;
        ECVec2 mCursorScreenPos;
        SnapValueSettings mSnapSettings;
        FrameBuffer* m_frameBuffer;
        int m_GizmoType{ 0 };
        bool IsWindowActive{ false };
        bool IsSnapping{ false };
    };
}
