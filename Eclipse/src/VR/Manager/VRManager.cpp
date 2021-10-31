#include "pch.h"
#include "VRManager.h"
#include "../Tools/pathtools.h"

namespace Eclipse
{
	bool VRManager::Init()
	{
		// Loading the SteamVR Runtime
		EVRInitError eError = VRInitError_None;
		m_pHMD = VR_Init(&eError, VRApplication_Scene);

		if (eError != VRInitError_None)
		{
			m_pHMD = NULL;
			char buffer[1024];
			sprintf_s(buffer, sizeof(buffer), "Unable to init VR runtime: %s",
				VR_GetVRInitErrorAsEnglishDescription(eError));

			return false;
		}

		m_strDriver = "No Driver";
		m_strDisplay = "No Display";

		m_strDriver = GetTrackedDeviceString(k_unTrackedDeviceIndex_Hmd, Prop_TrackingSystemName_String);
		m_strDisplay = GetTrackedDeviceString(k_unTrackedDeviceIndex_Hmd, Prop_SerialNumber_String);

		// Compositior makes sense of all of the VR Overlays and put them together to create a realistic VR visual
		// Failure of initialization means it can't perform its role of displaying intended visual
		if (!InitCompositor())
		{
			std::cout << "Failed to initialize VR Compositor!" << std::endl;
			return false;
		}

		VRInput()->SetActionManifestPath(Path_MakeAbsolute("../hellovr_actions.json", Path_StripFilename(Path_GetExecutablePath())).c_str());
		vr::VRInput()->GetActionHandle("/actions/demo/in/AnalogInput", &m_actionAnalogInput);

		vr::VRInput()->GetActionSetHandle("/actions/demo", &m_actionsetDemo);

		vr::VRInput()->GetInputSourceHandle("/user/hand/left", &m_rHand[Left].m_source);
		vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Left", &m_rHand[Left].m_actionPose);

		vr::VRInput()->GetInputSourceHandle("/user/hand/right", &m_rHand[Right].m_source);
		vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Right", &m_rHand[Right].m_actionPose);

		return true;
	}

	bool VRManager::InitCompositor()
	{
		EVRInitError peError = VRInitError_None;

		if (!VRCompositor())
		{
			printf("Compositor intialization failed. See log file for details\n");
			return false;
		}

		return true;
	}

	void VRManager::Shutdown()
	{
		if (m_pHMD)
		{
			VR_Shutdown();
			m_pHMD = NULL;
		}
	}

	void VRManager::ProcessVREvent(const VREvent_t& event)
	{
		switch (event.eventType)
		{
		case VREvent_TrackedDeviceDeactivated:
			printf("Device %u detached.\n", event.trackedDeviceIndex);
			break;
		case VREvent_TrackedDeviceUpdated:
			printf("Device %u updated.\n", event.trackedDeviceIndex);
			break;
		default:
			break;
		}
	}

	std::string VRManager::GetTrackedDeviceString(TrackedDeviceIndex_t unDevice, 
		TrackedDeviceProperty prop, TrackedPropertyError* peError)
	{
		uint32_t unRequiredBufferLen = VRSystem()->GetStringTrackedDeviceProperty(unDevice,
			prop, NULL, 0, peError);

		if (!unRequiredBufferLen)
			return "";

		char* pchBuffer = new char[unRequiredBufferLen];
		unRequiredBufferLen = VRSystem()->GetStringTrackedDeviceProperty(unDevice, prop,
			pchBuffer, unRequiredBufferLen, peError);
		std::string sResult = pchBuffer;
		delete []pchBuffer;

		return sResult;
	}

	glm::mat4 VRManager::GetHMDMatrixProjectionEye(Hmd_Eye nEye)
	{
		return glm::mat4();
	}

	glm::mat4 VRManager::GetHMDMatrixPoseEye(Hmd_Eye nEye)
	{
		return glm::mat4();
	}

	glm::mat4 VRManager::GetCurrentViewProjectionMatrix(Hmd_Eye nEye)
	{
		return glm::mat4();
	}

	void VRManager::UpdateHMDMatrixPose()
	{
	}
}

