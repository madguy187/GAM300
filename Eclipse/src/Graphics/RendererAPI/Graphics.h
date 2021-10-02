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
#include "ECS/ComponentManager/Components/MeshComponent.h"

const int maxParticles = 10000;

class World;

namespace Eclipse
{
    class Graphics
    {
        static void LoadShaders(std::string shaderFile);
        static void LoadModels();
        static void LoadTextures(std::string textureFile);
        static void LoadAllShaders();

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
        static void DeleteSprite(unsigned int id);
        static void DeleteAllSprites();

        //container for textures
        static std::unordered_map<std::string, Texture> textures;

        //container for sprites
        static std::multimap<unsigned int, MeshComponent*> sprites;
        static std::set<unsigned int> sortedID;

        //Container for shader programs and helper functions
        static std::unordered_map<std::string, Shader> shaderpgms;
        using shaderVec = std::vector<std::pair<std::string, std::string>>;
        static void initShaderpgms(std::string shdrpgm_name,
            std::string vtx_shdr, std::string frg_shdr);

        static modelIt FindModel(std::string);
        static shaderIt FindShaders(std::string);
    };
}

#endif /* GRAPHICS_H */

