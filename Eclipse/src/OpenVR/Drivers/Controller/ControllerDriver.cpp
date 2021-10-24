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

		// Telling OpenVR where to get my driver's Input Profile
		VRProperties()->SetStringProperty(props, Prop_InputProfilePath_String, "{EclipseEngine}/input/controller_profile.json");
		// Telling OpenVR what kind of device this is
		// Will be changing it constantly for testing purposes
		// End game, for our gam, it should be TrackedControllerRole_OptOut, I think LOL
		VRProperties()->SetInt32Property(props, Prop_ControllerRoleHint_Int32, ETrackedControllerRole::TrackedControllerRole_OptOut);
		// Sets up handler, using to send joystick commands to OpenVR with, in the Y direction (forward/backward)
		VRDriverInput()->CreateScalarComponent(props, "/input/joystick/y", &JoystickYHandle, EVRScalarType::VRScalarType_Absolute,
			EVRScalarUnits::VRScalarUnits_NormalizedTwoSided);

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