#include "pch.h"
#include "VRManager.h"

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

		return true;
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
}

