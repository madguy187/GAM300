#pragma once

#include "openvr.h"

namespace Eclipse
{
	using namespace vr;

	class bVRRenderModel
	{
	public:
		bVRRenderModel(const std::string& sRenderModelName);
		~bVRRenderModel();

		/**
		* Initialises OpenGL buffers for the model.
		* @param vrModel The model data retrieved from OpenVR API
		* @param vrDiffuseTexture The diffuse texture data retrieved from the OpenVR API
		* @return true if successful
		*/
		bool BInit(const RenderModel_t& vrModel,
			const RenderModel_TextureMap_t& vrDiffuseTexture);
		void Cleanup();

		/**
		* Called during rendering loop to draw the model.
		*/
		void Draw();
		const std::string& GetName() const { return m_sModelName; }

	private:
		GLuint m_glVertBuffer;
		GLuint m_glIndexBuffer;
		GLuint m_glVertArray;
		GLuint m_glTexture;
		GLsizei m_unVertexCount;
		std::string m_sModelName;
	};
}