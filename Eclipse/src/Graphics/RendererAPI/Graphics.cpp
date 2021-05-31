/**********************************************************************************
* \file            Graphics.cpp
*
* \brief        Definition of the Graphics class function(s).
*
* \author       Lin Yanping Rachel
*
* \email		l.yanpingrachel@digipen.edu
*
* \date			10 oct 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#define _USE_MATH_DEFINES

#include "pch.h"
#include "Graphics.h"

#include "imgui.h"
#include "type_ptr.hpp"
#include "matrix_transform_2d.hpp"
#include "vec.h"
#include "../src/ECS/World.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"

/*                                                   objects with file scope
----------------------------------------------------------------------------- */
using namespace rapidjson;

using namespace Eclipse;

std::map<std::string, Shader> Graphics::shaderpgms;
std::map<std::string, std::unique_ptr<IModel>> Graphics::models;
std::map<std::string, Texture> Graphics::textures;
std::map<std::string, Text> Graphics::Texts;
std::map<std::string, Dialogue> Graphics::dialogue;
std::vector<Dialogue> Graphics::currDialogue;
std::multimap<unsigned int, Sprite*> Graphics::sprites;
std::multimap<unsigned int, unsigned int> Graphics::interactables;
std::set<unsigned int> Graphics::sortedID;
std::bitset<2> Graphics::dialogueBits;
std::map<std::string, Particles> Graphics::particleType;
std::array<Particle, maxParticles> Graphics::particles;
std::vector<Graphics::Vertex> Graphics::particleVtx;

unsigned int Graphics::framebuffer;
unsigned int Graphics::framebufferTest;

// create a color attachment texture
unsigned int Graphics::textureColorbuffer;
static bool checker;

float Graphics::windowX;
float Graphics::windowY;

float Graphics::winWidth;
float Graphics::winHeight;

vec2 Graphics::frameBufferPos;

vec2 mousecoords;

GLdouble Graphics::timer = 0.5;

int StickCount = 0;
int StickID = 0;
bool NumebrTest = false;
bool mouseEditor = true;

extern glm::vec3 spherePos;
extern glm::vec3 sphereScale;
extern glm::vec3 sphereRot;

int createdID = 0;

void Graphics::load()
{
  //Loads all the shader programs listed in the shader file
  LoadShaders("src/Assets/Shaders/Shaders.json");

  //Loads all models listed
  LoadModels("src/Assets/meshes/Models.json");

  //Loads all textures listed
  LoadTextures("src/Assets/Textures/Textures.json");

  //Loads all particle types
  LoadParticles();
}

void Graphics::unload()
{
  for (auto it = shaderpgms.begin(); it != shaderpgms.end(); ++it)
  {
    it->second.DeleteShaderProgram();
  }

  for (auto it = models.begin(); it != models.end(); ++it)
  {
    it->second->DeleteModel();
  }

  for (auto it = textures.begin(); it != textures.end(); ++it)
  {
    it->second.DeleteTexture();
  }
}

void Graphics::LoadShaders(std::string shaderFile)
{
  Parser input;
  input.ParseFile(shaderFile);

  std::string shdrName, vtxName, frgName;

  for (auto& it : input.doc["shaders"].GetArray())
  {
    shdrName = (it)["shaderName"].GetString();
    vtxName = (it)["vtxShader"].GetString();
    frgName = (it)["frgShader"].GetString();

    initShaderpgms(shdrName, vtxName, frgName);
  }

}

void Graphics::LoadModels(std::string modelFile)
{
  Parser input;
  input.ParseFile(modelFile);

  std::string modelName;
  int index;

  for (auto& it : input.doc["models"].GetArray())
  {
    modelName = (it)["modelName"].GetString();
    index = (it)["modelIndex"].GetInt();

    models.emplace(modelName, ModelFactory::create(index));
  }

}

void Graphics::LoadTextures(std::string textureFile)
{
  Parser input;
  input.ParseFile(textureFile);

  std::string textureName;
  std::string path;
  glm::ivec2 spriteDimensions, spriteIndex;

  for (auto& it : input.doc["textures"].GetArray())
  {
    textureName = (it)["textureName"].GetString();
    path = (it)["path"].GetString();
    spriteDimensions = { (it)["spriteDimensions"][rapidjson::SizeType(0)].GetInt(),
                         (it)["spriteDimensions"][rapidjson::SizeType(1)].GetInt() };
    spriteIndex = { (it)["spriteIndex"][rapidjson::SizeType(0)].GetInt(),
                    (it)["spriteIndex"][rapidjson::SizeType(1)].GetInt() };

    Texture newTex(path);
    newTex.setSpriteWidth(spriteDimensions.x);
    newTex.setSpriteHeight(spriteDimensions.y);
    textures.emplace(textureName, newTex);
  }
}

void Graphics::LoadFonts(std::string FontFile)
{
  //Parser input;
  //input.ParseFile(FontFile);

  //std::string fontName;
  //std::string path;

  //for (auto& it : input.doc["fonts"].GetArray())
  //{
  //  fontName = (it)["fontName"].GetString();
  //  path = (it)["path"].GetString();

  //  //Fonts newFont(path);
  //  //fonts.emplace(fontName, newFont);
  //}
}

void Graphics::LoadParticles()
{
  Particles newParticle;

  particleType.emplace("Default", newParticle);
}

/******************************************************************************/
/*!
    This function sets up the shader program by loading the shader files and
    uses the shader program pipeline functionality implemented in class
    GLSLShader.
 */
 /******************************************************************************/
void Graphics::initShaderpgms(std::string shdrpgm_name,
  std::string vtx_shdr, std::string frg_shdr)
{
  std::vector<std::pair<GLenum, std::string>> shdr_files
  {
      std::make_pair(GL_VERTEX_SHADER, vtx_shdr),
      std::make_pair(GL_FRAGMENT_SHADER, frg_shdr)
  };

  Shader shdrpgm;
  shdrpgm.CompileLinkValidate(shdr_files);

  if (GL_FALSE == shdrpgm.IsLinked())
  {
    std::cout << "Unable to compile/link/validate shader programs\n";
    std::cout << shdrpgm.GetLog() << "\n";
    std::exit(EXIT_FAILURE);
  }

  // add compiled, linked, and validated shader program to std::map container GLApp::shdrpgms
  Graphics::shaderpgms[shdrpgm_name] = shdrpgm;
}

void Graphics::DeleteSprite(unsigned int id)
{
  //auto handle = _world->GetComponentManager().GetComponent<Sprite>(id);

  //if (!handle.has())
  //{
  //  return;
  //}

  //auto& targetSprite = handle.get();

  //for (auto iterator = std::begin(sprites); iterator != std::end(sprites); ++iterator)
  //{
  //  if (((*iterator).first == targetSprite.layerNum) && ((*iterator).second->ID == targetSprite.ID))
  //  {
  //    sprites.erase(iterator);
  //    sortedID.erase(targetSprite.ID);
  //    return;
  //  }
  //}
}

void Graphics::DeleteAllSprites()
{
  sprites.clear();
  sortedID.clear();
}
/******************************************************************************/
/*!
    This function loads the image using stb_image and creates the texture object.
    The function returns a handle to texture object to be passed to the shader
    program.
 */
 /******************************************************************************/
GLuint Graphics::setup_texobj(std::string pathname)
{
  int width = 0;
  int height = 0;
  int clrChannels;
  unsigned char* data = nullptr; // stbi_load(pathname.c_str(), &width, &height, &clrChannels, 0);

  if (data)
  {
    GLuint texobj_hdl;
    // define and initialize a handle to texture object that will encapsulate two-dimensional textures
    glCreateTextures(GL_TEXTURE_2D, 1, &texobj_hdl);
    // allocate GPU storage for texture image data loaded from file
    glTextureStorage2D(texobj_hdl, 1, GL_RGBA8, width, height);
    // copy image data from client memory to GPU texture buffer memory
    glTextureSubImage2D(texobj_hdl, 0, 0, 0, width, height,
      GL_RGBA, GL_UNSIGNED_BYTE, data);
    // client memory not required since image is buffered in GPU memory
    //stbi_image_free(data);

    return texobj_hdl;
  }
  else
  {
    std::string error = "hi";  //stbi_failure_reason();
    std::cout << "Texture could not be loaded! : " << error << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

typename Graphics::modelIt Graphics::FindModel(std::string name)
{
  return models.find(name);
}

typename Graphics::shaderIt Graphics::FindShaders(std::string name)
{
  return shaderpgms.find(name);
}

void Graphics::SortLayers()
{
  //auto& manager = _world->GetComponentManager();
  //auto lookup = manager.GetComponentLookup<Sprite>();

  //for (auto& pair : lookup)
  //{
  //  pair.second->ID = pair.first;
  //  auto it = Graphics::sortedID.find(pair.first);

  //  if (it == Graphics::sortedID.end())
  //  {
  //    Graphics::sprites.emplace(pair.second->layerNum, pair.second);
  //    Graphics::sortedID.emplace(pair.first);
  //  }
  //}
}
