#include "pch.h"
#include "DeviceProvider.h"

#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )

namespace Eclipse
{
	// Global, single instance, of the class that provides 
	// OpenVR with all of your devices.
	DeviceProvider deviceProvider;

	/**
	* This method returns an instance of your provider that OpenVR uses.
	**/
	HMD_DLL_EXPORT
	void* HmdDriverFactory(const char* interfaceName, int* returnCode)
	{
		if (!strcmp(interfaceName, IServerTrackedDeviceProvider_Version))
		{
			return &deviceProvider;
		}

		if (returnCode)
		{
			*returnCode = vr::VRInitError_Init_InterfaceNotFound;
		}

		return NULL;
	}
}
