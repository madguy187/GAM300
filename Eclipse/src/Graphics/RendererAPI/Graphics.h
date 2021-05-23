#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "glew.h"
#include "glfw3.h"
#include "glhelper.h"
#include <stb_image.h>
#include "glm.hpp"
#include "Shader.h""
#include "ModelFactory.h"
#include "Texture.h"
#include "AllComponents.h"
#include <vector>
#include <array>
#include <map>
#include <set>
#include <bitset>

#include "Vec.h"
#include "Particles.h"

#include "FrameBuffer.h"
#include "../src/ECS/SystemManager/Systems/System/System.h"

const int maxParticles = 10000;

class World;

class Graphics 
{
  static void LoadShaders(std::string shaderFile);
  static void LoadModels(std::string modelFile);
  static void LoadTextures(std::string textureFile);
  static void LoadFonts(std::string FontFile);
  static void LoadParticles();
  static void CreateObject(GLint model);
  static void CreateFrameBuffer();

  //Loads the image and creates texture object
  static GLuint setup_texobj(std::string pathname);

public:
  std::set<Entity> mEntities;
  static FrameBuffer* m_frameBuffer;

  static std::map<std::string, std::unique_ptr<IModel>> models;
  using modelIt = std::map<std::string, std::unique_ptr<IModel>>::iterator;
  using shaderIt = std::map<std::string, Shader>::iterator;
  using TextureIt = std::map<std::string, Texture>::iterator;

  static void load();
  static void init();
  static void update(double delta_time);
  static void draw();
  static void cleanup();
  static void unload();
  static void DeleteSprite(unsigned int id);
  static void DeleteAllSprites();

  //container for textures
  static std::map<std::string, Texture> textures;

  //container for objects
  static std::map<std::string, Object> objects;

  //container for static texts
  static std::map<std::string, Text> Texts;

  //container for dialogue texts
  static std::map<std::string, Dialogue> dialogue;
  static std::vector<Dialogue> currDialogue;

  //container for sprites
  static std::multimap<unsigned int, Sprite*> sprites;
  static std::set<unsigned int> sortedID;

  //container for interactables
  static std::multimap<unsigned int, unsigned int> interactables;

  struct Vertex
  {
    glm::vec2 Position;
    glm::vec4 Color;
    glm::vec2 UV;
  };

  //container for particles
  static std::map<std::string, Particles> particleType;
  static std::array<Particle, maxParticles> particles;
  static std::vector<Vertex> particleVtx;

  //Container for shader programs and helper functions
  static std::map<std::string, Shader> shaderpgms;
  using shaderVec = std::vector<std::pair<std::string, std::string>>;
  static void initShaderpgms(std::string shdrpgm_name,
    std::string vtx_shdr, std::string frg_shdr);

  static GLdouble timer;
  static modelIt FindModel(std::string);
  static shaderIt FindShaders(std::string);

  void TESTRENDER();

  void SortLayers();

  void DrawBuffers(unsigned int framebuffer);

  void CheckUniformLoc(Sprite& sprite);

  static unsigned int framebuffer;
  static unsigned int framebufferTest;
  static unsigned int textureColorbuffer;

  static float windowX;
  static float windowY;
  static float winWidth;
  static float winHeight;
  static vec2 frameBufferPos;

  static std::bitset<2> dialogueBits;
};

#endif /* GRAPHICS_H */
