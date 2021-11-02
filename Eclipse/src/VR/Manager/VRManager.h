#pragma once

#include "openvr.h"
#include "../Models/bVRRenderModel.h"

namespace Eclipse
{
	using namespace vr;

	//class VRManager
	//{
	//public:
	//	bool Init();
	//	bool InitCompositor();

	//	void Shutdown();

	//	void ProcessVREvent(const VREvent_t& event);
	//	void RenderFrame();

	//	void RenderControllerAxes();

	//	bool SetupStereoRenderTargets();
	//	void SetupCameras();

	//	void RenderStereoTargets();
	//	void RenderScene(Hmd_Eye neye);

	//	glm::mat4 GetHMDMatrixProjectionEye(Hmd_Eye nEye);
	//	glm::mat4 GetHMDMatrixPoseEye(Hmd_Eye nEye);
	//	glm::mat4 GetCurrentViewProjectionMatrix(Hmd_Eye nEye);
	//	void UpdateHMDMatrixPose();

	//	glm::mat4 ConvertSteamVRMatrixToMatrix4(const HmdMatrix34_t& matPose);
	//private:
	//	/**
	//	* Essential VR Instances
	//	**/
	//	IVRSystem* m_pHMD;
	//	std::string m_strDriver;
	//	std::string m_strDisplay;
	//	TrackedDevicePose_t m_rTrackedDevicePose[k_unMaxTrackedDeviceCount];
	//	glm::mat4 m_rmat4DevicePose[k_unMaxTrackedDeviceCount];

	//	struct ControllerInfo_t
	//	{
	//		VRInputValueHandle_t m_source{ k_ulInvalidInputValueHandle };
	//		VRActionHandle_t m_actionPose{ k_ulInvalidActionHandle };
	//		VRActionHandle_t m_actionHaptic{ k_ulInvalidActionHandle };
	//		glm::mat4 m_rmat4Pose;
	//		bool m_bShowController;
	//	};

	//	enum EHand
	//	{
	//		Left = 0,
	//		Right = 1,
	//	};
	//	ControllerInfo_t m_rHand[2];

	//	/**
	//	* OpenGL BookKeeping 
	//	**/
	//	int m_iTrackedControllerCount;
	//	int m_iTrackedControllerCount_Last;
	//	int m_iValidPoseCount;
	//	int m_iValidPoseCount_Last;
	//	bool m_bShowCubes;
	//	glm::vec2 m_vAnalogValue;

	//	// What classes we saw poses for this frame
	//	std::string m_strPoseClasses;
	//	// For each device, a character representing its class
	//	char m_rDevClassChar[k_unMaxTrackedDeviceCount];

	//	float m_fNearClip;
	//	float m_fFarClip;

	//	GLuint m_glControllerVertBuffer;
	//	GLuint m_unControllerVAO;
	//	unsigned int m_uiControllerVercount;

	//	glm::mat4 m_mat4HMDPose;
	//	glm::mat4 m_mat4eyePosLeft;
	//	glm::mat4 m_mat4eyePosRight;

	//	glm::mat4 m_mat4ProjectionCenter;
	//	glm::mat4 m_mat4ProjectionLeft;
	//	glm::mat4 m_mat4ProjectionRight;

	//	GLuint m_unControllerTransformProgramID;
	//	GLuint m_nControllerMatrixLocation;

	//	VRActionHandle_t m_actionHideCubes{ k_ulInvalidActionHandle };
	//	VRActionHandle_t m_actionHideThisController{ k_ulInvalidActionHandle };
	//	VRActionHandle_t m_actionTriggerHaptic{ k_ulInvalidActionHandle };
	//	VRActionHandle_t m_actionAnalogInput{ k_ulInvalidActionHandle };

	//	VRActionHandle_t m_actionsetDemo{ k_ulInvalidActionHandle };

	//	std::string GetTrackedDeviceString(TrackedDeviceIndex_t unDevice,
	//		TrackedDeviceProperty prop, TrackedPropertyError* peError = NULL);
	//};

	class VRManager
	{
	public:
		VRManager();
		bool Init();
		bool Update();
		bool Destroy();

		/* To retrieve the model of the tracked device from SteamVR */
		bVRRenderModel* FindOrLoadRenderModel(const char* renderModelString);

		std::string GetTrackedDeviceString(IVRSystem* pHmd, TrackedDeviceIndex_t unDevice,
			TrackedDeviceProperty prop, TrackedPropertyError* peError = NULL);

		~VRManager();

		struct FrameBufferDesc
		{
			GLuint m_nDepthBufferID{ 0 };
			GLuint m_nRenderTextureID{ 0 };
			GLuint m_nRenderFrameBufferID{ 0 };
			GLuint m_nResolveTextureID{ 0 };
			GLuint m_nResolveFrameBufferID{ 0 };
		};

	protected:
		/* Initialize VR Components */
		bool InitializeComponents();
		bool InitVRCompositor();
		bool InitRenderModels();
		bool InitEyeBuffer(int nWidth, int nHeight, FrameBufferDesc& fbDesc);

		void SetupCameras();
		bool SetupStereoRenderTargets();
		void SetupCompanionWindow();
		void SetupRenderModels();
		void SetupRenderModelForTrackedDevice(TrackedDeviceIndex_t unTrackedDeviceIndex);

		/* Render VR Components*/
		void RenderControllerAxes();
		void RenderToTV();
		void RenderStereoTargets();
		void RenderCompanionWindow();

		void RenderScene(Hmd_Eye nEye);

		glm::mat4 GetCurrentViewProjectionMatrix(Hmd_Eye nEye);
		glm::mat4 GetCurrentViewMatrix(Hmd_Eye nEye);
		glm::mat4 GetCurrentProjectionMatrix(Hmd_Eye nEye);

		void UpdateHMDMatrixPose();

		/* VR Utilities */
		glm::mat4 Mat4FromSteamVRMatrix(const HmdMatrix34_t& matPose);

		glm::vec3 GetControllerPosition(TrackedDeviceIndex_t trackedDeviceIndex);

		glm::vec3 GetControllerRaycastDirection(TrackedDeviceIndex_t trackedDeviceIndex);

		glm::mat4 GetHMDMatrixProjectionEye(Hmd_Eye nEye);
		glm::mat4 GetHMDMatrixPoseEye(Hmd_Eye nEye);

		/* Event Processing */
		void PollVREvent();
		void ProcessButtonEvent(VREvent_t event);

	private:
		FrameBufferDesc leftEyeDesc{};
		FrameBufferDesc rightEyeDesc{};

		/* VR System */
		IVRSystem* pHmd = NULL; // pointer to Head Mounted Display
		EVRInitError eError = VRInitError_None;
		IVRRenderModels* m_pRenderModels = NULL;
		bool m_rbShowTrackedDevice[vr::k_unMaxTrackedDeviceCount] = {};

		/* VR Picking */
		// ObjectPickingVR* Picker = nullptr;

		/* VR Shaders */
		// Shader* VRcontrollerShader = nullptr;
		// Shader* VRrendermodelShader = nullptr;
		// Shader* VRcompanionwindowShader = nullptr;
		
		// Shader* tvShader = nullptr;

		/* VR Cameras */
		// Camera* leftEyeCamera = nullptr;
		// Camera* rightEyeCamera = nullptr;

		/* VR FBOS for Eyes */
		// PostProcessing* leftEyeFBO = nullptr;
		// PostProcessing* rightEyeFBO = nullptr;

		/* VR Tracked Devices */
		TrackedDevicePose_t m_rTrackedDevicePose[k_unMaxTrackedDeviceCount] = {};
		glm::mat4 m_rmat4DevicePose[k_unMaxTrackedDeviceCount] = {};
		int m_iValidPoseCount{ 0 };
		int m_iValidPoseCount_Last{ 0 };

		std::string m_strPoseClasses{}; // What Classes we saw poses for this frame
		char m_rDevClassChar[k_unMaxTrackedDeviceCount] = {};

		/* VR Render Models */
		 std::vector<bVRRenderModel*> m_vecRenderModels;
		 bVRRenderModel* m_rTrackedDeviceToRenderModel[vr::k_unMaxTrackedDeviceCount];

		/* VR Controllers */
		int m_iTrackedControllerCount{ 0 };
		int m_iTrackedControllerCount_Last{ 0 };

		GLuint m_glControllerVertBuffer{ 0 };
		GLuint m_unControllerVAO{ 0 };
		unsigned int m_uiControllerVertCount{ 0 };

		/* Recommended Resolution */
		uint32_t m_nRenderWidth{ 0 };
		uint32_t m_nRenderHeight{ 0 };

		/* HMD Pose Matrix */
		glm::mat4 m_mat4HMDPose{};

		glm::mat4 m_mat4ProjectionLeft{};
		glm::mat4 m_mat4ProjectionRight{};
		glm::mat4 m_mat4eyePosLeft{};
		glm::mat4 m_mat4eyePosRight{};

		/* Matrix Locations */
		unsigned int m_vr_controllerlocation{ 0 };
		unsigned int m_vr_rendermodellocation{ 0 };

		/* For Companion Window */
		struct VertexDataWindow
		{
			glm::vec2 position{};
			glm::vec2 texCoord{};

			VertexDataWindow(const glm::vec2& pos, const glm::vec2 tex) :
				position{ pos }, texCoord{ tex } {}
		};

		GLuint m_unCompanionWindowVAO{ 0 };
		GLuint m_glCompanionWindowIDVertBuffer{ 0 };
		GLuint m_glCompanionWindowIDIndexBuffer{ 0 };
		unsigned int m_uiCompanionWindowIndexSize{ 0 };

		glm::mat4 view{};

		/* Object Picking */
		TrackedDeviceIndex_t pickCtrlIndex{ 0 };
		bool bTriggerDown{ false };
		bool bHit{ false };
	};
}