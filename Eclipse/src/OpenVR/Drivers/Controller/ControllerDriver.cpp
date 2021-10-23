#include "pch.h"
#include "ControllerDriver.h"

namespace Eclipse
{
	EVRInitError ControllerDriver::Activate(uint32_t unObjID)
	{
		// Unique Driver ID
		DriverID = unObjID;

		// Retrieves container object that stores information about my driver
		PropertyContainerHandle_t props = VRProperties()->TrackedDeviceToPropertyContainer(DriverID);

		// Tell OpenVR where to get my driver's Input Profile
		VRProperties()->SetStringProperty(props, Prop_InputProfilePath_String, "{EclipseEngine}/input/controller_profile.json");

		return VRInitError_None;
	}

	void ControllerDriver::Deactivate()
	{
	}

	void ControllerDriver::EnterStandby()
	{
	}

	void* ControllerDriver::GetComponent(const char* pchComponentNameAndVersion)
	{
		return nullptr;
	}

	void ControllerDriver::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize)
	{
	}

	DriverPose_t ControllerDriver::GetPose()
	{
		return DriverPose_t();
	}

	void ControllerDriver::RunFrame()
	{
	}
}