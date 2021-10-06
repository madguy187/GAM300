#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "glew.h"
#include "glfw3.h"
#include <stb_image.h>
#include "glm.hpp"
#include "Shader.h"
#include "ModelFactory.h"
#include "Texture.h"
#include <vector>
#include <array>
#include <map>
#include <set>
#include <bitset>
#include "Particles.h"
#include "FrameBuffer.h"
#include "../src/ECS/SystemManager/Systems/System/System.h"
#include "Graphics/OpenGL/OpenGL_Context.h"
#include "ECS/ComponentManager/Components/MaterialComponent.h"

namespace Eclipse
{
	class Graphics
	{
		static void LoadShaders(std::string shaderFile);
		static void LoadModels();

		//Loads the image and creates texture object
		static GLuint setup_texobj(std::string pathname);

	public:
		std::set<Entity> mEntities;
		static std::unordered_map<std::string, std::unique_ptr<IModel>> models;
		using modelIt = std::unordered_map<std::string, std::unique_ptr<IModel>>::iterator;
		using shaderIt = std::unordered_map<std::string, Shader>::iterator;
		using TextureIt = std::unordered_map<std::string, Texture>::iterator;

		static void load();
		static void unload();
		static void ThreadLoadShaders();

		//container for textures
		static std::multimap<std::string, Texture> textures;

		//Container for shader programs and helper functions
		static std::unordered_map<std::string, Shader> shaderpgms;
		using shaderVec = std::vector<std::pair<std::string, std::string>>;
		static void initShaderpgms(bool in , std::string shdrpgm_name, std::string vtx_shdr, std::string frg_shdr , std::string geo = "");
		static modelIt FindModel(std::string);
		static shaderIt FindShaders(std::string);
		static Texture FindTextures(std::string);
		static void GetTexuresForModels(std::string in, MaterialComponent com);
	};
}

#endif /* GRAPHICS_H */

