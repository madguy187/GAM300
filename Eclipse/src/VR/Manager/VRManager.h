#pragma once

#include "openvr.h"

namespace Eclipse
{
	using namespace vr;

	class VRManager
	{
	public:
		bool Init();
		bool InitCompositor();

		void Shutdown();

		void ProcessVREvent(const VREvent_t& event);
		void RenderFrame();

		void RenderControllerAxes();

		bool SetupStereoRenderTargets();
		void SetupCameras();

		void RenderStereoTargets();
		void RenderScene(Hmd_Eye neye);

		glm::mat4 GetHMDMatrixProjectionEye(Hmd_Eye nEye);
		glm::mat4 GetHMDMatrixPoseEye(Hmd_Eye nEye);
		glm::mat4 GetCurrentViewProjectionMatrix(Hmd_Eye nEye);
		void UpdateHMDMatrixPose();

		glm::mat4 ConvertSteamVRMatrixToMatrix4(const HmdMatrix34_t& matPose);
	private:
		/**
		* Essential VR Instances
		**/
		IVRSystem* m_pHMD;
		std::string m_strDriver;
		std::string m_strDisplay;
		TrackedDevicePose_t m_rTrackedDevicePose[k_unMaxTrackedDeviceCount];
		glm::mat4 m_rmat4DevicePose[k_unMaxTrackedDeviceCount];

		struct ControllerInfo_t
		{
			VRInputValueHandle_t m_source{ k_ulInvalidInputValueHandle };
			VRActionHandle_t m_actionPose{ k_ulInvalidActionHandle };
			VRActionHandle_t m_actionHaptic{ k_ulInvalidActionHandle };
			glm::mat4 m_rmat4Pose;
			bool m_bShowController;
		};

		enum EHand
		{
			Left = 0,
			Right = 1,
		};
		ControllerInfo_t m_rHand[2];

		/**
		* OpenGL BookKeeping 
		**/
		int m_iTrackedControllerCount;
		int m_iTrackedControllerCount_Last;
		int m_iValidPoseCount;
		int m_iValidPoseCount_Last;
		bool m_bShowCubes;
		glm::vec2 m_vAnalogValue;

		// What classes we saw poses for this frame
		std::string m_strPoseClasses;
		// For each device, a character representing its class
		char m_rDevClassChar[k_unMaxTrackedDeviceCount];

		GLuint m_glControllerVertBuffer;
		GLuint m_unControllerVAO;
		unsigned int m_uiControllerVercount;

		glm::mat4 m_mat4HMDPose;
		glm::mat4 m_mat4eyePosLeft;
		glm::mat4 m_mat4eyePosRight;

		glm::mat4 m_mat4ProjectionCenter;
		glm::mat4 m_mat4ProjectionLeft;
		glm::mat4 m_mat4ProjectionRight;

		GLuint m_unControllerTransformProgramID;
		GLuint m_nControllerMatrixLocation;

		VRActionHandle_t m_actionHideCubes{ k_ulInvalidActionHandle };
		VRActionHandle_t m_actionHideThisController{ k_ulInvalidActionHandle };
		VRActionHandle_t m_actionTriggerHaptic{ k_ulInvalidActionHandle };
		VRActionHandle_t m_actionAnalogInput{ k_ulInvalidActionHandle };

		VRActionHandle_t m_actionsetDemo{ k_ulInvalidActionHandle };

		std::string GetTrackedDeviceString(TrackedDeviceIndex_t unDevice,
			TrackedDeviceProperty prop, TrackedPropertyError* peError = NULL);
	};
}