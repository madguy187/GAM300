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
		VRProperties()->SetStringProperty(props, Prop_InputProfilePath_String, "{Eclipse}/input/controller_profile.json");
		// Telling OpenVR what kind of device this is
		// Will be changing it constantly for testing purposes
		// End game, for our gam, it should be TrackedControllerRole_OptOut, I think LOL
		VRProperties()->SetInt32Property(props, Prop_ControllerRoleHint_Int32, ETrackedControllerRole::TrackedControllerRole_Treadmill);
		// Sets up handler, using to send joystick commands to OpenVR with, in the Y direction (forward/backward)
		VRDriverInput()->CreateScalarComponent(props, "/input/joystick/y", &JoystickYHandle, EVRScalarType::VRScalarType_Absolute,
			EVRScalarUnits::VRScalarUnits_NormalizedTwoSided);
		// Sets up handler, using to send trackpad commands to OpenVR with, in the Y direction
		VRDriverInput()->CreateScalarComponent(props, "/input/trackpad/y", &TrackpadYHandle, EVRScalarType::VRScalarType_Absolute,
			EVRScalarUnits::VRScalarUnits_NormalizedTwoSided);
		// VRScalarUnits_NormalizedTwoSided - Scalar values range from -1 to 1 inclusively. 
		// Joysticks and trackpads generally use this value.
		// VRScalarType_Absolute - The scalar values are updated with values on an absolute scale. 
		// Joysticks, trackpads, and triggers are all examples of absolute scalar values.
		VRDriverInput()->CreateScalarComponent(props, "/input/joystick/x", &JoystickXHandle, EVRScalarType::VRScalarType_Absolute,
			EVRScalarUnits::VRScalarUnits_NormalizedTwoSided);
		VRDriverInput()->CreateScalarComponent(props, "/input/trackpad/x", &TrackpadXHandle, EVRScalarType::VRScalarType_Absolute,
			EVRScalarUnits::VRScalarUnits_NormalizedTwoSided);

		return VRInitError_None;
	}

	void ControllerDriver::Deactivate()
	{
		DriverID = k_unTrackedDeviceIndexInvalid;
	}

	void ControllerDriver::EnterStandby() {}

	void* ControllerDriver::GetComponent(const char* pchComponentNameAndVersion)
	{
		if (!strcmp(IVRDriverInput_Version, pchComponentNameAndVersion))
			return this;

		return NULL;
	}

	void ControllerDriver::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize)
	{
		if (unResponseBufferSize > 0)
			pchResponseBuffer[0] = 0;
	}

	DriverPose_t ControllerDriver::GetPose()
	{
		// Return default Pose for now
		DriverPose_t pose = { 0 };
		pose.poseIsValid = false;
		pose.result = TrackingResult_Calibrating_OutOfRange;
		pose.deviceIsConnected = true;

		HmdQuaternion_t quat{};
		quat.w = 1;
		quat.x = 0;
		quat.y = 0;
		quat.z = 0;

		pose.qWorldFromDriverRotation = quat;
		pose.qDriverFromHeadRotation = quat;

		return pose;
	}

	void ControllerDriver::RunFrame()
	{
		// Based on the scalar units used, the range is -1 to 1
		// Move Forward
		VRDriverInput()->UpdateScalarComponent(JoystickYHandle, 0.95f, 0.0);
		// Move Forward
		VRDriverInput()->UpdateScalarComponent(TrackpadYHandle, 0.95f, 0.0);
		// Can change both values to move sideways
		VRDriverInput()->UpdateScalarComponent(JoystickXHandle, 0.0f, 0.0);
		VRDriverInput()->UpdateScalarComponent(TrackpadXHandle, 0.0f, 0.0);
	}
}
