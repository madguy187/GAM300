#include "pch.h"
#include "VRManager.h"
#include "../Tools/pathtools.h"

#define MSAA_SAMPLES 8

namespace Eclipse
{
	//bool VRManager::Init()
	//{
	//	// Loading the SteamVR Runtime
	//	EVRInitError eError = VRInitError_None;
	//	m_pHMD = VR_Init(&eError, VRApplication_Scene);

	//	if (eError != VRInitError_None)
	//	{
	//		m_pHMD = NULL;
	//		char buffer[1024];
	//		sprintf_s(buffer, sizeof(buffer), "Unable to init VR runtime: %s",
	//			VR_GetVRInitErrorAsEnglishDescription(eError));

	//		return false;
	//	}

	//	m_strDriver = "No Driver";
	//	m_strDisplay = "No Display";

	//	m_strDriver = GetTrackedDeviceString(k_unTrackedDeviceIndex_Hmd, Prop_TrackingSystemName_String);
	//	m_strDisplay = GetTrackedDeviceString(k_unTrackedDeviceIndex_Hmd, Prop_SerialNumber_String);

	//	// Compositior makes sense of all of the VR Overlays and put them together to create a realistic VR visual
	//	// Failure of initialization means it can't perform its role of displaying intended visual
	//	if (!InitCompositor())
	//	{
	//		std::cout << "Failed to initialize VR Compositor!" << std::endl;
	//		return false;
	//	}

	//	VRInput()->SetActionManifestPath(Path_MakeAbsolute("../hellovr_actions.json", Path_StripFilename(Path_GetExecutablePath())).c_str());
	//	vr::VRInput()->GetActionHandle("/actions/demo/in/AnalogInput", &m_actionAnalogInput);

	//	vr::VRInput()->GetActionSetHandle("/actions/demo", &m_actionsetDemo);

	//	vr::VRInput()->GetInputSourceHandle("/user/hand/left", &m_rHand[Left].m_source);
	//	vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Left", &m_rHand[Left].m_actionPose);

	//	vr::VRInput()->GetInputSourceHandle("/user/hand/right", &m_rHand[Right].m_source);
	//	vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Right", &m_rHand[Right].m_actionPose);

	//	return true;
	//}

	//bool VRManager::InitCompositor()
	//{
	//	EVRInitError peError = VRInitError_None;

	//	if (!VRCompositor())
	//	{
	//		printf("Compositor intialization failed. See log file for details\n");
	//		return false;
	//	}

	//	return true;
	//}

	//void VRManager::Shutdown()
	//{
	//	if (m_pHMD)
	//	{
	//		VR_Shutdown();
	//		m_pHMD = NULL;
	//	}
	//}

	//void VRManager::ProcessVREvent(const VREvent_t& event)
	//{
	//	switch (event.eventType)
	//	{
	//	case VREvent_TrackedDeviceDeactivated:
	//		printf("Device %u detached.\n", event.trackedDeviceIndex);
	//		break;
	//	case VREvent_TrackedDeviceUpdated:
	//		printf("Device %u updated.\n", event.trackedDeviceIndex);
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//void VRManager::RenderFrame()
	//{
	//}

	//void VRManager::RenderControllerAxes()
	//{
	//}

	//bool VRManager::SetupStereoRenderTargets()
	//{
	//	return false;
	//}

	//void VRManager::SetupCameras()
	//{
	//}

	//void VRManager::RenderStereoTargets()
	//{
	//}

	//void VRManager::RenderScene(Hmd_Eye neye)
	//{
	//}

	//std::string VRManager::GetTrackedDeviceString(TrackedDeviceIndex_t unDevice, 
	//	TrackedDeviceProperty prop, TrackedPropertyError* peError)
	//{
	//	uint32_t unRequiredBufferLen = VRSystem()->GetStringTrackedDeviceProperty(unDevice,
	//		prop, NULL, 0, peError);

	//	if (!unRequiredBufferLen)
	//		return "";

	//	char* pchBuffer = new char[unRequiredBufferLen];
	//	unRequiredBufferLen = VRSystem()->GetStringTrackedDeviceProperty(unDevice, prop,
	//		pchBuffer, unRequiredBufferLen, peError);
	//	std::string sResult = pchBuffer;
	//	delete []pchBuffer;

	//	return sResult;
	//}

	//glm::mat4 VRManager::GetHMDMatrixProjectionEye(Hmd_Eye nEye)
	//{
	//	return glm::mat4();
	//}

	//glm::mat4 VRManager::GetHMDMatrixPoseEye(Hmd_Eye nEye)
	//{
	//	return glm::mat4();
	//}

	//glm::mat4 VRManager::GetCurrentViewProjectionMatrix(Hmd_Eye nEye)
	//{
	//	return glm::mat4();
	//}

	//void VRManager::UpdateHMDMatrixPose()
	//{
	//}
	//glm::mat4 VRManager::ConvertSteamVRMatrixToMatrix4(const HmdMatrix34_t& matPose)
	//{
	//	return glm::mat4();
	//}

	VRManager::VRManager() {}

	VRManager::~VRManager()
	{
		VR_Shutdown();
	}
	
	bool VRManager::Init()
	{
		EVRInitError eError = VRInitError_None;
		pHmd = VR_Init(&eError, VRApplication_Scene);

		if (eError != VRInitError_None)
		{
			pHmd = NULL;
			std::cout
				<< "OpenVR: Unable to int VR runtime: "
				<< VR_GetVRInitErrorAsEnglishDescription(eError)
				<< std::endl;
		}

		if (!InitializeComponents())
			return false;

		/* Initialize Shaders*/

		/*this->VRcontrollerShader = new Shader("controller", c_VRcontrollerShaderVert, c_VRcontrollerShaderFrag);
		assert(this->VRcontrollerShader != nullptr);
		m_vr_controllerlocation = VRcontrollerShader->getUniLocation("matrix");

		this->VRrendermodelShader = new Shader("rendermodel", c_VRrendermodelShaderVert, c_VRrendermodelShaderFrag);
		assert(this->VRrendermodelShader != nullptr);
		m_vr_rendermodellocation = VRrendermodelShader->getUniLocation("matrix");

		this->VRcompanionwindowShader = new Shader("companionwindow", c_VRcompanionwindowShaderVert, c_VRcompanionwindowShaderFrag);
		assert(this->VRcompanionwindowShader != nullptr);*/

		/* Initialize Scene */
		// MainScene::Init();
		SetupCameras();
		SetupStereoRenderTargets();
		SetupCompanionWindow();
		SetupRenderModels();

		/*dynamicWorld->removeRigidBody(Player->rigidBody);

		delete msaaEffect; msaaEffect = nullptr;
		delete HdrEffect; HdrEffect = new Hdr(m_nRenderWidth, m_nRenderHeight);
		delete BloomEffect; BloomEffect = new PostProcessing(m_nRenderWidth, m_nRenderHeight);
		delete NoEffects; NoEffects = new PostProcessing(m_nRenderWidth, m_nRenderHeight);

		Picker = new ObjectPickingVR(pHmd, dynamicWorld);*/

		return true;
	}
	
	bool VRManager::Update()
	{
		/* Event Polling for VR */
		PollVREvent();

		if (pHmd)
		{
			glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);
			/*this->CubeDepthMap->RenderToDepthmap(depthShader);
			RenderObjects(depthShader);
			this->CubeDepthMap->UnbindFBO();*/

			RenderStereoTargets();
			RenderCompanionWindow();

			Texture_t leftEyeTexture = { (void*)(uintptr_t)leftEyeDesc.m_nResolveTextureID, TextureType_OpenGL, ColorSpace_Gamma };
			VRCompositor()->Submit(Eye_Left, &leftEyeTexture);
			Texture_t rightEyeTexture = { (void*)(uintptr_t)rightEyeDesc.m_nResolveTextureID, TextureType_OpenGL, ColorSpace_Gamma };
			VRCompositor()->Submit(Eye_Right, &rightEyeTexture);
		}

		glFinish();
		UpdateHMDMatrixPose();

		return true;
	}
	
	bool VRManager::Destroy()
	{
		return true;
	}
	
	bVRRenderModel* VRManager::FindOrLoadRenderModel(const char* renderModelString)
	{
		bVRRenderModel* renderModel = NULL;

		for (std::vector<bVRRenderModel*>::iterator i = m_vecRenderModels.begin(); 
			i != m_vecRenderModels.end(); i++)
		{
			if (!stricmp((*i)->GetName().c_str(), renderModelString))
			{
				renderModel = *i;
				break;
			}
		}

		if (!renderModel)
		{
			RenderModel_t* r_model;
			EVRRenderModelError error;

			while (true)
			{
				error = VRRenderModels()->LoadRenderModel_Async(renderModelString, &r_model);

				if (error != VRRenderModelError_Loading)
					break;

				Sleep(1);
			}

			if (error != VRRenderModelError_None)
			{
				std::cout 
					<< "OpenVR: Unable to load render model " 
					<< renderModelString << ": " 
					<< VRRenderModels()->GetRenderModelErrorNameFromEnum(error) 
					<< std::endl;
				return NULL;
			}

			RenderModel_TextureMap_t* r_texture;

			while (true)
			{
				error = VRRenderModels()->LoadTexture_Async(r_model->diffuseTextureId, &r_texture);

				if (error != VRRenderModelError_Loading)
					break;

				Sleep(1);
			}

			if (error != VRRenderModelError_None)
			{
				std::cout 
					<< "OpenVR: Unable to load render texture " 
					<< renderModelString << ": "
					<< VRRenderModels()->GetRenderModelErrorNameFromEnum(error) 
					<< std::endl;
				return NULL;
			}

			renderModel = new bVRRenderModel(renderModelString);

			if (!renderModel->BInit(*r_model, *r_texture))
			{
				std::cout 
					<< "OpenVR: Unable to create OpenGL model from " 
					<< renderModelString 
					<< std::endl;
				renderModel = NULL;
			}
			else
			{
				m_vecRenderModels.push_back(renderModel);
			}

			VRRenderModels()->FreeRenderModel(r_model);
			VRRenderModels()->FreeTexture(r_texture);
		}

		return renderModel;
	}

	std::string VRManager::GetTrackedDeviceString(IVRSystem* pHmd, TrackedDeviceIndex_t unDevice, TrackedDeviceProperty prop, TrackedPropertyError* peError)
	{
		uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, 
			prop, NULL, 0, peError);
		if (unRequiredBufferLen == 0)
			return "";

		char* pchBuffer = new char[unRequiredBufferLen];
		unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop,
			pchBuffer, unRequiredBufferLen, peError);
		std::string sResult = pchBuffer;
		delete[] pchBuffer;
		return sResult;
	}
	
	bool VRManager::InitializeComponents()
	{
		if (!InitRenderModels())
			return false;

		if (!InitVRCompositor())
			return false;

		return true;
	}
	
	bool VRManager::InitVRCompositor()
	{
		// Takes all the various OpenVR/SteamVR overlays and puts them together
		// Enables the SteamVR ingame menu
		if (!VRCompositor())
		{
			std::cout << "OpenVR: Unable to initialize VR Compositor" << std::endl;
			return false;
		}

		return true;
	}
	
	bool VRManager::InitRenderModels()
	{
		m_pRenderModels = (IVRRenderModels*)VR_GetGenericInterface(IVRRenderModels_Version,
			&eError);

		if (!m_pRenderModels)
		{
			std::cout << "OpenVR: Failed to intialize Render Models!" << std::endl;
			VR_Shutdown();
			return false;
		}

		return true;
	}
	
	bool VRManager::InitEyeBuffer(int nWidth, int nHeight, FrameBufferDesc& fbDesc)
	{
		glGenFramebuffers(1, &fbDesc.m_nRenderFrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, fbDesc.m_nRenderFrameBufferID);

		glGenRenderbuffers(1, &fbDesc.m_nDepthBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, fbDesc.m_nDepthBufferID);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLES, GL_DEPTH_COMPONENT, nWidth, nHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbDesc.m_nDepthBufferID);

		glGenTextures(1, &fbDesc.m_nRenderTextureID);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fbDesc.m_nRenderTextureID);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLES, GL_RGBA32F, nWidth, nHeight, true);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, fbDesc.m_nRenderTextureID, 0);

		glGenFramebuffers(1, &fbDesc.m_nResolveFrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, fbDesc.m_nResolveFrameBufferID);

		glGenTextures(1, &fbDesc.m_nResolveTextureID);
		glBindTexture(GL_TEXTURE_2D, fbDesc.m_nResolveTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); //
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbDesc.m_nResolveTextureID, 0);

		// check FBO status
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
			return false;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}
	
	/*Initialize Eye Cameras*/
	void VRManager::SetupCameras()
	{
		m_mat4ProjectionLeft = GetHMDMatrixProjectionEye(Eye_Left);
		m_mat4ProjectionRight = GetHMDMatrixProjectionEye(Eye_Right);
		m_mat4eyePosLeft = GetHMDMatrixPoseEye(Eye_Left);
		m_mat4eyePosRight = GetHMDMatrixPoseEye(Eye_Right);
	}
	
	bool VRManager::SetupStereoRenderTargets()
	{
		if (!pHmd)
			return false;

		pHmd->GetRecommendedRenderTargetSize(&m_nRenderWidth, &m_nRenderHeight);

		InitEyeBuffer(m_nRenderWidth, m_nRenderHeight, leftEyeDesc);
		InitEyeBuffer(m_nRenderWidth, m_nRenderHeight, rightEyeDesc);

		return true;
	}
	
	void VRManager::SetupCompanionWindow()
	{
		if (!pHmd)
			return;

		std::vector<VertexDataWindow> vVerts;

		// left eye verts                  // Position         // TexCoord
		vVerts.push_back(VertexDataWindow(glm::vec2(-1, -1), glm::vec2(0, 0)));
		vVerts.push_back(VertexDataWindow(glm::vec2(0, -1), glm::vec2(1, 0)));
		vVerts.push_back(VertexDataWindow(glm::vec2(-1, 1), glm::vec2(0, 1)));
		vVerts.push_back(VertexDataWindow(glm::vec2(0, 1), glm::vec2(1, 1)));

		// right eye verts
		vVerts.push_back(VertexDataWindow(glm::vec2(0, -1), glm::vec2(0, 0)));
		vVerts.push_back(VertexDataWindow(glm::vec2(1, -1), glm::vec2(1, 0)));
		vVerts.push_back(VertexDataWindow(glm::vec2(0, 1), glm::vec2(0, 1)));
		vVerts.push_back(VertexDataWindow(glm::vec2(1, 1), glm::vec2(1, 1)));

		GLushort vIndices[] = { 0, 1, 3,   0, 3, 2,   4, 5, 7,   4, 7, 6 };
		m_uiCompanionWindowIndexSize = _countof(vIndices);

		glGenVertexArrays(1, &m_unCompanionWindowVAO);
		glBindVertexArray(m_unCompanionWindowVAO);

		glGenBuffers(1, &m_glCompanionWindowIDVertBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_glCompanionWindowIDVertBuffer);
		glBufferData(GL_ARRAY_BUFFER, 
			vVerts.size() * sizeof(VertexDataWindow), &vVerts[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_glCompanionWindowIDIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glCompanionWindowIDIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			m_uiCompanionWindowIndexSize * sizeof(GLushort), &vIndices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 
			sizeof(VertexDataWindow), (void*)offsetof(VertexDataWindow, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
			sizeof(VertexDataWindow), (void*)offsetof(VertexDataWindow, texCoord));

		glBindVertexArray(0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	void VRManager::SetupRenderModels()
	{
		memset(m_rTrackedDeviceToRenderModel, 0, sizeof(m_rTrackedDeviceToRenderModel));

		if (!pHmd)
			return;

		for (uint32_t unTrackedDevice = vr::k_unTrackedDeviceIndex_Hmd + 1; 
			unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++)
		{
			if (!pHmd->IsTrackedDeviceConnected(unTrackedDevice))
				continue;

			SetupRenderModelForTrackedDevice(unTrackedDevice);
		}
	}
	
	void VRManager::SetupRenderModelForTrackedDevice(TrackedDeviceIndex_t unTrackedDeviceIndex)
	{
		if (unTrackedDeviceIndex >= k_unMaxTrackedDeviceCount)
			return;

		// Find a model we have already set up
		std::string sRenderModelName = GetTrackedDeviceString(pHmd,
			unTrackedDeviceIndex, Prop_RenderModelName_String);

		bVRRenderModel* pRenderModel = FindOrLoadRenderModel(sRenderModelName.c_str());

		if (!pRenderModel)
		{
			std::string sTrackingSystemName = GetTrackedDeviceString(pHmd, unTrackedDeviceIndex, vr::Prop_TrackingSystemName_String);
			std::cout << "OpenVR: Unable to load render model for tracked device with name " << sTrackingSystemName << std::endl;
		}
		else
		{
			m_rTrackedDeviceToRenderModel[unTrackedDeviceIndex] = pRenderModel;
			m_rbShowTrackedDevice[unTrackedDeviceIndex] = true;
		}
	}
	
	void VRManager::RenderControllerAxes()
	{
		// Dont update controllers if input is not available
		if (!pHmd->IsInputAvailable())
			return;

		std::vector<float> vertdataarray;

		m_uiControllerVertCount = 0;
		m_iTrackedControllerCount = 0;

		for (vr::TrackedDeviceIndex_t unTrackedDevice = k_unTrackedDeviceIndex_Hmd + 1; 
			unTrackedDevice < k_unMaxTrackedDeviceCount; ++unTrackedDevice)
		{
			if (!pHmd->IsTrackedDeviceConnected(unTrackedDevice))
				continue;

			if (pHmd->GetTrackedDeviceClass(unTrackedDevice) != TrackedDeviceClass_Controller)
				continue;

			m_iTrackedControllerCount += 1;

			if (!m_rTrackedDevicePose[unTrackedDevice].bPoseIsValid)
				continue;

			const glm::mat4& mat = m_rmat4DevicePose[unTrackedDevice];

			glm::vec4 center = mat * glm::vec4(0, 0, 0, 1);

			for (int i = 0; i < 3; ++i)
			{
				glm::vec3 color(0, 0, 0);
				glm::vec4 point(0, 0, 0, 1);
				point[i] += 0.05f;  // offset in X, Y, Z
				color[i] = 1.0;  // R, G, B
				point = mat * point;
				vertdataarray.push_back(center.x);
				vertdataarray.push_back(center.y);
				vertdataarray.push_back(center.z);

				vertdataarray.push_back(color.x);
				vertdataarray.push_back(color.y);
				vertdataarray.push_back(color.z);

				vertdataarray.push_back(point.x);
				vertdataarray.push_back(point.y);
				vertdataarray.push_back(point.z);

				vertdataarray.push_back(color.x);
				vertdataarray.push_back(color.y);
				vertdataarray.push_back(color.z);

				m_uiControllerVertCount += 2;
			}

			glm::vec4 start = mat * glm::vec4(0, 0, -0.02f, 1);
			glm::vec4 end = mat * glm::vec4(0, 0, -39.f, 1);
			glm::vec3 color(.92f, .92f, .71f);

			vertdataarray.push_back(start.x); 
			vertdataarray.push_back(start.y); 
			vertdataarray.push_back(start.z);

			vertdataarray.push_back(color.x);
			vertdataarray.push_back(color.y); 
			vertdataarray.push_back(color.z);

			vertdataarray.push_back(end.x); 
			vertdataarray.push_back(end.y); 
			vertdataarray.push_back(end.z);

			vertdataarray.push_back(color.x); 
			vertdataarray.push_back(color.y); 
			vertdataarray.push_back(color.z);

			m_uiControllerVertCount += 2;
		}

		// Setup the VAO - Vertex Array Object the first time through.
		if (m_unControllerVAO == 0)
		{
			glGenVertexArrays(1, &m_unControllerVAO);
			glBindVertexArray(m_unControllerVAO);

			glGenBuffers(1, &m_glControllerVertBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_glControllerVertBuffer);

			GLuint stride = 2 * 3 * sizeof(float);
			uintptr_t offset = 0;

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);

			offset += sizeof(glm::vec3);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);

			glBindVertexArray(0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_glControllerVertBuffer);

		// set vertex data if we have some
		if (vertdataarray.size() > 0)
		{
			glBufferData(GL_ARRAY_BUFFER, 
				sizeof(float) * vertdataarray.size(), 
				&vertdataarray[0], 
				GL_STREAM_DRAW);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void VRManager::RenderToTV()
	{
		// tvEffect->BindFBO();
		glViewport(0, 0, 500, 400);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// modelShader->Use();
		// tvCamera->Update();
		// RenderObjects(modelShader);
		// RenderLights(modelShader);
		// tvEffect->UnbindFBO();
	}
	
	/* Renders left eye and right eye to framebufers */
	void VRManager::RenderStereoTargets()
	{
		// Render to TV in the VR Environment
		RenderToTV();

		glEnable(GL_MULTISAMPLE);

		/* Left Eye */
		glBindFramebuffer(GL_FRAMEBUFFER, leftEyeDesc.m_nRenderFrameBufferID);
		glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);

		// CubeDepthMap->BindDepthTexture();
		RenderScene(Eye_Left);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_MULTISAMPLE);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, leftEyeDesc.m_nRenderFrameBufferID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, leftEyeDesc.m_nResolveFrameBufferID);

		glBlitFramebuffer(0, 0, m_nRenderWidth, 
			m_nRenderHeight, 0, 0, m_nRenderWidth, m_nRenderHeight,
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glEnable(GL_MULTISAMPLE);

		/* Right Eye */
		glBindFramebuffer(GL_FRAMEBUFFER, rightEyeDesc.m_nRenderFrameBufferID);
		glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);

		// CubeDepthMap->BindDepthTexture();
		RenderScene(Eye_Right);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_MULTISAMPLE);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, rightEyeDesc.m_nRenderFrameBufferID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rightEyeDesc.m_nResolveFrameBufferID);

		glBlitFramebuffer(0, 0, m_nRenderWidth,
			m_nRenderHeight, 0, 0, m_nRenderWidth, m_nRenderHeight,
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	
	void VRManager::RenderCompanionWindow()
	{
		glDisable(GL_DEPTH_TEST);
		// glViewport(0, 0, application->w(), application->h());

		glBindVertexArray(m_unCompanionWindowVAO);
		// VRcompanionwindowShader->Use();

		// render left eye (first half of index array )
		glBindTexture(GL_TEXTURE_2D, leftEyeDesc.m_nResolveTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glDrawElements(GL_TRIANGLES, m_uiCompanionWindowIndexSize / 2, GL_UNSIGNED_SHORT, 0);

		// render right eye (second half of index array )
		glBindTexture(GL_TEXTURE_2D, rightEyeDesc.m_nResolveTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glDrawElements(GL_TRIANGLES, m_uiCompanionWindowIndexSize / 2,
			GL_UNSIGNED_SHORT, (const void*)(uintptr_t)(m_uiCompanionWindowIndexSize));

		glBindVertexArray(0);
		glUseProgram(0);
	}
	
	void VRManager::RenderScene(vr::Hmd_Eye nEye)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		bool bIsInputAvailable = pHmd->IsInputAvailable();

		// modelShader->Use();
		view = GetCurrentViewMatrix(nEye);
		// glUniformMatrix4fv(modelShader->getUniLocation("projection"), 1, GL_FALSE, GetCurrentProjectionMatrix(nEye).get());
		// glUniformMatrix4fv(modelShader->getUniLocation("view"), 1, GL_FALSE, view.get());
		// modelShader->setVec3("viewPos", glm::vec3(view[3], view[7], view[11]));

		// RenderObjects(modelShader);
		// RenderLights(modelShader);

		// modelShader->setInt("isTVQuad", 1);
		// tvEffect->Bind();
		// RenderTVSceneQuad();
		// modelShader->setInt("isTVQuad", 0);

		// VRrendermodelShader->Use();

		for (uint32_t unTrackedDevice = 0; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++)
		{
			if (!m_rTrackedDeviceToRenderModel[unTrackedDevice] || !m_rbShowTrackedDevice[unTrackedDevice])
				continue;

			const vr::TrackedDevicePose_t& pose = m_rTrackedDevicePose[unTrackedDevice];
			if (!pose.bPoseIsValid)
				continue;

			if (!bIsInputAvailable && pHmd->GetTrackedDeviceClass(unTrackedDevice) == vr::TrackedDeviceClass_Controller)
				continue;

			const glm::mat4& matDeviceToTracking = m_rmat4DevicePose[unTrackedDevice];
			glm::mat4 matMVP = GetCurrentViewProjectionMatrix(nEye) * matDeviceToTracking;
			// glUniformMatrix4fv(VRrendermodelShader->getUniLocation("matrix"), 1, GL_FALSE, matMVP.get());

			m_rTrackedDeviceToRenderModel[unTrackedDevice]->Draw();

		}

		glUseProgram(0);
	}
	
	glm::mat4 VRManager::GetCurrentViewProjectionMatrix(Hmd_Eye nEye)
	{
		glm::mat4 matMVP;

		if (nEye == Eye_Left)
		{				// p									// V
			matMVP = m_mat4ProjectionLeft * m_mat4eyePosLeft * m_mat4HMDPose;
		}
		else if (nEye == Eye_Right)
		{
			matMVP = m_mat4ProjectionRight * m_mat4eyePosRight * m_mat4HMDPose;
		}

		return matMVP;
	}
	
	glm::mat4 VRManager::GetCurrentViewMatrix(Hmd_Eye nEye)
	{
		glm::mat4 matV;

		if (nEye == Eye_Left)
		{
			matV = m_mat4eyePosLeft * m_mat4HMDPose;
		}
		else if (nEye == Eye_Right)
		{
			matV = m_mat4eyePosRight * m_mat4HMDPose;
		}

		return matV;
	}
	
	glm::mat4 VRManager::GetCurrentProjectionMatrix(Hmd_Eye nEye)
	{
		glm::mat4 matP;

		if (nEye == Eye_Left)
		{
			matP = m_mat4ProjectionLeft;
		}
		else if (nEye == Eye_Right)
		{
			matP = m_mat4ProjectionRight;
		}

		return matP;
	}
	
	void VRManager::UpdateHMDMatrixPose()
	{
		if (!pHmd)
			return;

		VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, 
			k_unMaxTrackedDeviceCount, NULL, 0);

		m_iValidPoseCount = 0;
		m_strPoseClasses = "";

		for (int nDevice = 0; nDevice < k_unMaxTrackedDeviceCount; ++nDevice)
		{
			if (m_rTrackedDevicePose[nDevice].bPoseIsValid)
			{
				m_iValidPoseCount++;
				m_rmat4DevicePose[nDevice] = Mat4FromSteamVRMatrix(m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
				if (m_rDevClassChar[nDevice] == 0)
				{
					switch (pHmd->GetTrackedDeviceClass(nDevice))
					{
					case TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C'; break;
					case TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H'; break;
					case TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I'; break;
					case TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'G'; break;
					case TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T'; break;
					default:                                   m_rDevClassChar[nDevice] = '?'; break;
					}
				}

				m_strPoseClasses += m_rDevClassChar[nDevice];
			}
		}

		if (m_rTrackedDevicePose[k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
		{
			m_mat4HMDPose = m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
			m_mat4HMDPose = glm::inverse(m_mat4HMDPose);
			// m_mat4HMDPose.invert();
		}
	}
	
	glm::mat4 VRManager::Mat4FromSteamVRMatrix(const HmdMatrix34_t& matPose)
	{
		glm::mat4 pose(
			matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
			matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
			matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
			matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
		);

		return pose;
	}
	
	glm::vec3 VRManager::GetControllerPosition(TrackedDeviceIndex_t trackedDeviceIndex)
	{
		const glm::mat4& mat = m_rmat4DevicePose[trackedDeviceIndex];
		return glm::vec3(mat[12], mat[13], mat[14]);
	}
	
	glm::vec3 VRManager::GetControllerRaycastDirection(TrackedDeviceIndex_t trackedDeviceIndex)
	{
		const glm::mat4& mat = m_rmat4DevicePose[trackedDeviceIndex];
		return glm::vec3(-mat[8], -mat[9], -mat[10]);
	}
	
	glm::mat4 VRManager::GetHMDMatrixProjectionEye(Hmd_Eye nEye)
	{
		if (!pHmd)
			return glm::mat4();

		float m_fNearClip = 0.05f;
		float m_fFarClip = 100.f;
		
		HmdMatrix44_t mat = pHmd->GetProjectionMatrix(nEye, m_fNearClip, m_fFarClip);

		return glm::mat4(
			mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
			mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
			mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
			mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
		);
	}
	
	glm::mat4 VRManager::GetHMDMatrixPoseEye(Hmd_Eye nEye)
	{
		if (!pHmd)
			return glm::mat4();

		HmdMatrix34_t matEyeRight = pHmd->GetEyeToHeadTransform(nEye);

		glm::mat4 matrixObj(
			matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
			matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
			matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
			matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
		);

		return glm::inverse(matrixObj);
	}
	
	/* Rendering - Call in Loop */
	void VRManager::PollVREvent()
	{
		VREvent_t event;

		if (pHmd->PollNextEvent(&event, sizeof(event)))
		{
			switch (event.eventType)
			{
			case VREvent_TrackedDeviceActivated:
				// Add rendering for device model
				SetupRenderModelForTrackedDevice(event.trackedDeviceIndex);
				std::cout 
					<< "OpenVR: Device " 
					<< event.trackedDeviceIndex 
					<< " attached." 
					<< std::endl;
				break;
			case VREvent_TrackedDeviceDeactivated:
				std::cout 
					<< "OpenVR: Device " 
					<< event.trackedDeviceIndex 
					<< " detached." 
					<< std::endl;
				break;
			case VREvent_TrackedDeviceUpdated:
				std::cout 
					<< "OpenVR: Device " 
					<< event.trackedDeviceIndex 
					<< " updated." 
					<< std::endl;
				break;
			default:
				break;
			}
		}

		for (TrackedDeviceIndex_t unDevice = 0; 
			unDevice < k_unMaxTrackedDeviceCount; unDevice++)
		{
			VRControllerState_t state;

			if (pHmd->GetControllerState(unDevice, &state, sizeof(state)))
				m_rbShowTrackedDevice[unDevice] = state.ulButtonPressed == 0;
		}

		ProcessButtonEvent(event);
	}
	
	void VRManager::ProcessButtonEvent(VREvent_t event)
	{
		VRControllerState_t state;

		switch (event.data.controller.button)
		{

		case k_EButton_SteamVR_Trigger:
			switch (event.eventType)
			{
			case VREvent_ButtonPress:
				// bHit = Picker->pickBody(GetControllerPosition(event.trackedDeviceIndex), GetControllerRaycastDirection(event.trackedDeviceIndex));
				pickCtrlIndex = event.trackedDeviceIndex;

				if (pHmd->GetControllerState(event.trackedDeviceIndex, &state, sizeof(state)))
				{
					bTriggerDown = !(state.ulButtonPressed == 0);
				}

				break;
			case VREvent_ButtonUnpress:
				bTriggerDown = false;
				bHit = false;
				// Picker->removePickingConstraint();
				break;
			case VREvent_ButtonTouch:
				break;
			}
			break;

		case k_EButton_SteamVR_Touchpad:
			switch (event.eventType)
			{
			case VREvent_ButtonPress:
				break;
			case VREvent_ButtonUnpress:
				break;
			case VREvent_ButtonTouch:
				break;
			case VREvent_ButtonUntouch:
				break;
			}
			break;

		default:
			// MOVE PICKED BODY
			if (bTriggerDown && bHit)
			{
				// Picker->movePickedBody(GetControllerPosition(pickCtrlIndex), GetControllerRaycastDirection(pickCtrlIndex));
			}
			break;
		}
	}
}
