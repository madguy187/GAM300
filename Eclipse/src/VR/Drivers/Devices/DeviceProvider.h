#pragma once

#include "../Controller/ControllerDriver.h"

namespace Eclipse
{
	using namespace vr;

	/**
	* Instantiates all the device drivers implemented.
	* This class will create instances each of them and informs OpenVR about all the devices.
	**/
	class DeviceProvider : public IServerTrackedDeviceProvider
	{
	public:
		/**
		* Initialize and add all drivers to OpenVR here.
		**/
		EVRInitError Init(IVRDriverContext* pDriverContext) override;

		/**
		* Called before our drivers are unloaded
		**/
		void Cleanup() override;

		/**
		* Returns version of the openVR interface this driver works with
		**/
		const char* const* GetInterfaceVersions() override;

		/**
		* Updates drivers every frame
		**/
		void RunFrame() override;

		/**
		* Rreturns true if standby mode should be blocked
		**/
		bool ShouldBlockStandbyMode() override;

		/**
		* Called when OpenVR
		**/
		void EnterStandby() override;

		/**
		* Updates drivers every frame
		**/
		void LeaveStandby() override;

	private:
		// For now, we only have one controller driver
		ControllerDriver* controllerDriver;
	};
}
