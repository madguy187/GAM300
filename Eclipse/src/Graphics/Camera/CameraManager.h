#pragma once
#include "pch.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/SystemManager/Systems/System/RenderSystem/RenderSystem.h"

namespace Eclipse
{
    class CameraManager
    {
        const ECVec3 EDITORCAM_INITPOS = ECVec3{ 0.0f, 20.0f, 40.0f };
        const ECVec3 EDITORCAM_INITROT = ECVec3{ -40.0f, -90.0f, 0.0f };

        const ECVec3 GAMECAM_INITPOS = ECVec3{ 0.0f, 0.0f, 30.0f };
        const ECVec3 GAMECAM_INITROT = ECVec3{ 0.0f, -90.0f, 0.0f };

        const ECVec3 TOPCAM_INITPOS = ECVec3{ 0.0f, 60.0f, 0.0f };
        const ECVec3 TOPCAM_INITROT = ECVec3{ -90.0f, 90.0f, 0.0f };
        const ECVec3 TOPCAM_INITSCALE = ECVec3{ 100.0f, 1.0f, 1.0f };

        const ECVec3 BOTTOMCAM_INITPOS = ECVec3{ 0.0f, -60.0f, 0.0f };
        const ECVec3 BOTTOMCAM_INITROT = ECVec3{ 90.0f, 90.0f, 0.0f };
        const ECVec3 BOTTOMCAM_INITSCALE = ECVec3{ 100.0f, 1.0f, 1.0f };

        const ECVec3 LEFTCAM_INITPOS = ECVec3{ -60.0f, 0.0f, 0.0f };
        const ECVec3 LEFTCAM_INITROT = ECVec3{ 0.0f, 0.0f, 0.0f };
        const ECVec3 LEFTCAM_INITSCALE = ECVec3{ 100.0f, 1.0f, 1.0f };

        const ECVec3 RIGHTCAM_INITPOS = ECVec3{ 60.0f, 0.0f, 0.0f };
        const ECVec3 RIGHTCAM_INITROT = ECVec3{ 0.0f, 180.0f, 0.0f };
        const ECVec3 RIGHTCAM_INITSCALE = ECVec3{ 100.0f, 1.0f, 1.0f };

        /*************************************************************************
          Camera bitset layout
          0: Move Right								: button A
          1: Move Left								: button D
          2: Move Front								: button W
          3: Move Back								: button S
          4: Pitch Up (Rotation around x-axis)		: button R
          5: Pitch Down (Rotation around x-axis)	: button F
          6: Yaw Left (Rotation around y-axis)		: button Q
          7: Yaw Right (Rotation around y-axis)		: button E
          8: "Zoom In" (FOV)						: button Z
          9: "Zoom Out" (FOV)						: button X
          10: Move Up								: button T
          11: Move Down								: button G
        *************************************************************************/
        std::bitset<12> input;
        //Mesh editor camera has same controls as editor camera
        std::bitset<12> meshInput;

        /*************************************************************************
          View Cameras bitset layout

          Orthographic projection
          0: Move Left								: button A
          1: Move Right								: button D
          2: Move Up								: button W
          3: Move Down								: button S
          4: Zoom In								: button Z
          5: Zoom Out								: button X

          Perspective projection
          0: Move Left								: button A
          1: Move Right								: button D
          2: Move Up								: button W
          3: Move Down								: button S
          4: Move Front								: button Z
          5: Move Back								: button X
          6: "Zoom In" (FOV)						: button R
          7: "Zoom Out" (FOV)						: button F
        *************************************************************************/
        std::bitset<8> viewInput;

        /*************************************************************************
          Material Camera bitset layout
          0: "Zoom In" (FOV)						: button Z
          1: "Zoom Out" (FOV)						: button X
        *************************************************************************/
        std::bitset<2> materialInput;

        std::map<CameraComponent::CameraType, unsigned int> cameraList;

        unsigned int editorCamID = MAX_ENTITY;
        unsigned int gameCamID = MAX_ENTITY;
    public:
        void CreateEditorCamera();
        unsigned int GetEditorCameraID();

        void CreateGameCamera();
        unsigned int GetGameCameraID();

        void CreatePerspectiveCamera(CameraComponent::CameraType _camType);

        void CreateViewCamera(CameraComponent::CameraType _camType);
        void SetViewCameraValues(CameraComponent::CameraType _camType, TransformComponent& _transform);
        void SetViewCameraProjectionType(CameraComponent& _camera, CameraComponent::ProjectionType _projType);
        void SetViewCameraProjectionType(CameraComponent::CameraType _camType, CameraComponent::ProjectionType _projType);

        unsigned int GetCameraID(CameraComponent::CameraType _camType);

        void ComputeViewDirection(CameraComponent& _camera, TransformComponent& _transform);
        void ComputeViewMtx(CameraComponent& _camera, TransformComponent& _transform);
        void ComputePerspectiveMtx(CameraComponent& _camera);
        void UpdateEditorCamera(TransformComponent& _transform);
        void UpdateMeshCamera(TransformComponent& _transform);

        void CheckCameraInput();
        void CheckMeshCameraInput();
        void CheckViewCameraInput();
        void CheckMaterialCameraInput();
        void UpdateViewCamera(CameraComponent& _camera, TransformComponent& _transform);
        void UpdateMaterialCamera(TransformComponent& _transform);

        void SetCameraSpeed(float newSpeed);
        void SetNearPlane(CameraComponent& _camera, float _nearPlane);
        void SetFarPlane(CameraComponent& _camera, float _farPlane);

        std::bitset<12>& GetInput();
        std::bitset<8>& GetViewInput();
        std::bitset<12>& GetMeshInput();
        std::bitset<2>& GetMaterialInput();

        void ResetScene();
        void ReInitCameraList(CameraComponent::CameraType _camType, unsigned int ID);
    };
}