#pragma once

#include "openvr_driver.h"

namespace Eclipse
{
	using namespace vr;

	/**
	* Controls the behaviour of the controller.
	* Tells OpenVR what my controller has (buttons, joystick, trackpad, etc.)
	* & informs OpenVR when the state of your controller changes (e.g. when a button is pressed)
	**/
	class ControllerDriver : public ITrackedDeviceServerDriver
	{
	public:
		/**
		* Initialize Controller
		* Gives OpenVR information about my controller
		* & set up handles to inform OpenVR when controller state changes
		**/
		EVRInitError Activate(uint32_t unObjID);

		/**
		* Uninitialize controller
		**/
		void Deactivate();

		/**
		* Tell hardware to go into standby mode - low power mode
		**/
		void EnterStandby();

		/**
		* Acts as an input device, so it will return the IVRDriverInput class
		* It could return other component classes if it had more functionality
		* E.g. overlays, UI functionality
		**/
		void* GetComponent(const char* pchComponentNameAndVersion);

		/**
		* The set of valid requests is entirely up to the driver and the client to figure out, 
		* as is the format of the response. Responses that exceed the length of the supplied buffer 
		* should be truncated and null terminated
		**/
		void DebugRequest(const char* pchRequest, char* pchResponseBuffer, 
			uint32_t unResponseBufferSize);

		/**
		* Returns pose of device.
		* It is an object that contains position, rotation, velocity and angular velocity of your device
		**/
		DriverPose_t GetPose();

		/**
		* Retrieve state of device and update OpenVR if anything has changed. Called every frame.
		**/
		void RunFrame();

	private:
		uint32_t DriverID;
		VRInputComponentHandle_t JoystickYHandle;
		VRInputComponentHandle_t TrackpadYHandle;
		VRInputComponentHandle_t JoystickXHandle;
		VRInputComponentHandle_t TrackpadXHandle;
	};
}
