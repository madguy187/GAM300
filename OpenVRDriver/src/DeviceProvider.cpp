#include "../include/DeviceProvider.h"

EVRInitError DeviceProvider::Init(IVRDriverContext* pDriverContext)
{
	EVRInitError initError = InitServerDriverContext(pDriverContext);

	if (initError != VRInitError_None)
		return initError;

	// Logging into Steam's log file
	VRDriverLog()->Log("Initializing Eclipse controller, this is real this is me");

	controllerDriver = new ControllerDriver();
	// Need to add "_controller" to controller driver name...
	// Add devices like this
	VRServerDriverHost()->TrackedDeviceAdded("Eclipse_controller",
		TrackedDeviceClass_Controller, controllerDriver);

	return VRInitError_None;
}

void DeviceProvider::Cleanup()
{
	delete controllerDriver;
	controllerDriver = NULL;
}

const char* const* DeviceProvider::GetInterfaceVersions()
{
	return k_InterfaceVersions;
}

void DeviceProvider::RunFrame()
{
	controllerDriver->RunFrame();
}

bool DeviceProvider::ShouldBlockStandbyMode()
{
	return false;
}

void DeviceProvider::EnterStandby() {}

void DeviceProvider::LeaveStandby() {}
