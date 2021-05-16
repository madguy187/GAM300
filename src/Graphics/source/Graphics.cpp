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
#include "../include/Graphics.h"
#include "../include/GLHelper.h"
#include <cmath>
#include <fstream>
#include "imgui.h"
#include <string>
#include <istream>

#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr
#include "glm/gtx/matrix_transform_2d.hpp""
#include "../include/Vec.h"
#include "../src/ECS/World.h"
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
using namespace rapidjson;


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
World* Graphics::_world = nullptr;
int StickCount = 0;
int StickID = 0;
bool NumebrTest = false;
bool mouseEditor = true;

GameViewW* Graphics::m_frameBuffer;
extern glm::vec3 spherePos;
extern glm::vec3 sphereScale;
extern glm::vec3 sphereRot;

void Graphics::load()
{
  //Loads all the shader programs listed in the shader file
  LoadShaders("Shaders/Shaders.json");

  //Loads all models listed
  LoadModels("meshes/Models.json");

  //Loads all textures listed
  LoadTextures("Textures/Textures.json");

  //Loads all particle types
  LoadParticles();
}

/******************************************************************************/
/*!
    Initialize function that reads and initializes data from a scene file and
    initializes the camera. Also invokes print_specs() to access OpenGL context
    information that is printed to the screen.
 */
 /******************************************************************************/
void Graphics::init()
{
  std::cout << "INIT" << std::endl;
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glViewport(0, 0, GLHelper::width, GLHelper::height);
  //GLHelper::print_specs();

  //GameViewW GameViewBuffer(GLHelper::width, GLHelper::height);

  //m_frameBuffer = new GameViewW(GLHelper::width, GLHelper::height);
  CreateFrameBuffer();
}

/******************************************************************************/
/*!
    Update function for Graphics
 */
 /******************************************************************************/
void Graphics::update(double fixed)
{
  // first, update camera
  //Graphics::camera2d.update(GLHelper::ptr_window, delta_time);

  int modelSelector = -1;
  ImGui::Begin("Create Objects");
  if (ImGui::Combo("Models", &modelSelector,
    "Square\0Circle\0Triangle\0Lines\0Sphere\0Cube\0Cylinder\0Cone\0Torus\0Pyramid"))
  {
    CreateObject(modelSelector);

    for (int i = 0; i <= 2; ++i)
    {
      ImGui::Separator();
    }
  }

  ImGui::Begin("Properties");
  ImGui::DragFloat3("Scale", (float*)&sphereScale, 0.2f, 0.0f, 0.0f);
  ImGui::DragFloat3("Translate", (float*)&spherePos, 0.2f, 0.0f, 0.0f);
  ImGui::DragFloat3("Rotate", (float*)&sphereRot, 0.2f, 0.0f, 0.0f);
  ImGui::End();
  ImGui::End();
}

/******************************************************************************/
/*!
    Draw function for Graphics
 */
 /******************************************************************************/
void Graphics::draw()
{
  //int keyT = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_T);

  if (!mouseEditor)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Original gray background
    //glClearColor(static_cast<GLclampf>(0.44), 
    //    static_cast<GLclampf>(0.565), static_cast<GLclampf>(0.604), 
    //    static_cast<GLclampf>(1.0));

    glClearColor(static_cast<GLclampf>(0.22),
      static_cast<GLclampf>(0.28), static_cast<GLclampf>(0.30),
      static_cast<GLclampf>(1.0));
    //world.Render();
  }
  else
  {
    //std::cout << "DRAWING ..." << std::endl;
    ////Note: ImGui might cause crashes depending on the rendering order. Always
    //// call ImGui separately or after rendering to the default framebuffer
    //glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer->GetGameViewBuffer());
    //glClear(GL_COLOR_BUFFER_BIT);

    ////world.Render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

    ImGui::Begin("Game View");

    ImGui::GetWindowDrawList()->AddImage(
      (void*)(textureColorbuffer),
      ImVec2(ImGui::GetCursorScreenPos()),
      ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowContentRegionMax().x,

        ImGui::GetCursorScreenPos().y + ImGui::GetWindowContentRegionMax().y), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();


    //ImVec2 pos = ImGui::GetCursorScreenPos();

    //mousecoords = { pos.x, pos.y };
    //ImGui::GetWindowDrawList()->AddImage(
    //  (void*)(textureColorbuffer),
    //  ImVec2(ImGui::GetCursorScreenPos()),
    //  ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowContentRegionMax().x,
    //    ImGui::GetCursorScreenPos().y + ImGui::GetWindowContentRegionMax().y), ImVec2(0, 1), ImVec2(1, 0));

    //winWidth = ImGui::GetWindowWidth();
    //winHeight = ImGui::GetWindowHeight();

    //windowX = (ImGui::GetWindowWidth() / 2) + ImGui::GetCursorScreenPos().x;
    //windowY = (ImGui::GetWindowHeight() / 2) + ImGui::GetCursorScreenPos().y;
    //frameBufferPos = vec2{ ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y };
    //ImGui::End();

    //GameViewW::ShowWindow(world, *m_frameBuffer);


  }
}

void Graphics::cleanup()
{

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


void Graphics::CreateObject(GLint model)
{
  //auto& entManager = world.GetEntityManager();
  //auto& compManager = world.GetComponentManager();
  //auto id = entManager.CreateEntity().GetID();
  //compManager.AddComponent<Transform>(id);
  //compManager.AddComponent<IsActive>(id);
  //compManager.AddComponent<Sprite>(id);
  //compManager.AddComponent<RigidBody>(id);
  //compManager.AddComponent<RegisterEvent>(id);
  ////compManager.AddComponent<BoundingBox>(id);

  //auto& sprite = compManager.GetComponent<Sprite>(id).get();
  //auto& transform = compManager.GetComponent<Transform>(id).get();

  //switch (model)
  //{
  //case 0:
  //{
  //  sprite.shaderRef = shaderpgms.find("shdrpgm");
  //  sprite.modelRef = models.find("square");
  //  sprite.isQuad = true;
  //}
  //break;
  //case 1:
  //  sprite.shaderRef = shaderpgms.find("shdrpgm");
  //  sprite.modelRef = models.find("circle");
  //  break;
  //case 2:
  //{
  //  sprite.shaderRef = shaderpgms.find("shdrpgm");
  //  sprite.modelRef = models.find("triangle");
  //}
  //break;
  //case 3:
  //{
  //  sprite.shaderRef = shaderpgms.find("shdrpgm");
  //  sprite.modelRef = models.find("lines");
  //}
  //break;

  //case 4:
  //{
  //  sprite.shaderRef = shaderpgms.find("shader3DShdrpgm");
  //  sprite.modelRef = models.find("sphere");
  //}
  //break;

  //case 5:
  //{
  //  sprite.shaderRef = shaderpgms.find("shader3DShdrpgm");
  //  sprite.modelRef = models.find("cube");
  //}
  //break;

  //case 6:
  //{
  //  sprite.shaderRef = shaderpgms.find("shader3DShdrpgm");
  //  sprite.modelRef = models.find("cylinder");
  //}
  //break;

  //case 7:
  //{
  //  sprite.shaderRef = shaderpgms.find("shader3DShdrpgm");
  //  sprite.modelRef = models.find("cone");
  //}
  //break;

  //case 8:
  //{
  //  sprite.shaderRef = shaderpgms.find("shader3DShdrpgm");
  //  sprite.modelRef = models.find("torus");
  //}
  //break;

  //case 9:
  //{
  //  sprite.shaderRef = shaderpgms.find("shader3DShdrpgm");
  //  sprite.modelRef = models.find("pyramid");
  //}
  //break;

  //}
}

void Graphics::CreateFrameBuffer()
{
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  glGenTextures(1, &textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GLHelper::width, GLHelper::height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GLHelper::width, GLHelper::height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::RegisterWorld(World* world)
{
  _world = world;
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

void Graphics::CreateMonkeytick(World* world, int _flag)
{
  //if (_flag & COLLISION_BOTTOM)
  //{
  //  auto& comMM = _world->GetComponentManager();
  //  auto _PlayercomMM = comMM.GetComponentLookup<Player>();

  //  for (auto& pair : _PlayercomMM)
  //  {
  //    auto& __player = *(pair.second);

  //    if (StickCount == 1 && (__player.airtime == false) && (_flag & COLLISION_BOTTOM))
  //    {
  //      // create stick
  //      CreateObject(6, (*world));

  //      PlayerInput.set(6, 1);
  //      PlayerInput.set(9, 1);

  //    }
  //  }
  //}
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
  int width, height, clrChannels;
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


//void Graphics::Print_Numbers(int number, vec3 location, int size, int duration, int colour, float velocity, int id)
//{
//  (void)colour;
//  (void)velocity;
//  (void)id;
//
//  int Offset = 50;
//  int n = (int)number;
//  int index = 0;
//  int numbers[256];
//
//  if (n == 0)
//  {
//    numbers[index++] = n;
//  }
//  while (n)
//  {
//    numbers[index++] = n % 10;
//    n /= 10;
//  }
//
//  index--;
//
//  while (index >= 0)
//  {
//
//    vec3 pos = { location.x, (location.y + Offset) ,0 };
//    vec3 scale = { (float)size,(float)size,0 };
//
//    auto& entManager = _world->GetEntityManager();
//    auto& compManager = _world->GetComponentManager();
//    auto id = entManager.CreateEntity().GetID();
//
//    compManager.AddComponent<Transform>(id);
//    compManager.AddComponent<IsActive>(id);
//    compManager.AddComponent<Sprite>(id);
//    compManager.AddComponent<RigidBody>(id);
//    compManager.AddComponent<NUMBER>(id);
//
//    auto& sprite = compManager.GetComponent<Sprite>(id).get();
//    auto& GetNumber = compManager.GetComponent<NUMBER>(id).get();
//    auto& NumberTransform = compManager.GetComponent<Transform>(id).get();
//
//    GetNumber._run = true;
//
//    sprite.shaderRef = shaderpgms.find("shdrpgm");
//    sprite.modelRef = models.find("square");
//    sprite.isQuad = true;
//    sprite.hasTexture = true;
//    sprite.textureRef = textures.find("numberspritesheet");
//    sprite.layerNum = 40;
//
//    GetNumber.health = (float)duration; //lifespan
//    sprite.textureIdx.x = (float)(numbers[index] * 1);
//    sprite.textureIdx.y = 1;
//    NumberTransform.position.x = pos.x - (scale.x * index);
//    NumberTransform.position.y = pos.y;
//
//    auto& _rigid = compManager.GetComponent<RigidBody>(id).get();
//    RigidBodyComponent::set(&_rigid, BodyType::Dynamic);
//    _rigid.gravity = 0.0f;
//    _rigid.force.y = 300.0f;
//
//    index--;
//  }
//}
//
//void Graphics::CreateDeathZone(World& world)
//{
//  extern std::bitset<4> toggle;
//
//  auto& entManager = world.GetEntityManager();
//  auto& compManager = world.GetComponentManager();
//  auto id = entManager.CreateEntity().GetID();
//
//  compManager.AddComponent<Transform>(id);
//  compManager.AddComponent<IsActive>(id);
//  compManager.AddComponent<Sprite>(id);
//  compManager.AddComponent<BoundingBox>(id);
//  compManager.AddComponent<DeathZone>(id);
//  compManager.AddComponent<RigidBody>(id);
//
//  auto& sprite = compManager.GetComponent<Sprite>(id).get();
//  sprite.shaderRef = shaderpgms.find("shdrpgm");
//  sprite.modelRef = models.find("square");
//  sprite.isQuad = true;
//  sprite.transparency = 0.25f;
//  sprite.hasTexture = false;
//  sprite.layerNum = 40;
//
//  auto& Body = compManager.GetComponent<RigidBody>(id).get();
//  RigidBodyComponent::set(&Body, BodyType::Dynamic);
//  Body.gravity = 0.0f;
//  Body._CollisionType = CollisionType::DeathZone;
//
//  auto& _scale = compManager.GetComponent<Transform>(id).get();
//  _scale.scale = { 100, 100 , 0 };
//
//  std::string myName = "DeathZone ";
//  EntityNameTable::AddNameToTable(id, myName);
//
//  auto& _SetToggle = compManager.GetComponent<DeathZone>(id).get();
//  _SetToggle.ToggleDeathZone = false;
//
//  isCreated.set(0, 1);
//
//  toggle.set(2, 0);
//  toggle.set(1, 1);
//  toggle.set(0, 0);
//}
//
//void Graphics::Print_Alphabet(const char* String, float Location_x, float Location_y, float size, int duration)
//{
//  int index = 0;
//  char characters[256];
//
//  while (String[index] != NULL)
//  {
//    characters[index] = String[index];
//    index++;
//  }
//
//  index--;
//
//  while (index >= 0)
//  {
//    vec3 pos = { Location_x, Location_y ,0 };
//    vec3 scale = { size,size,0 };
//
//    if (characters[index] > 90)
//    {
//      characters[index] -= 32;
//    }
//
//    if (characters[index] != 32)
//    {
//      auto& entManager = _world->GetEntityManager();
//      auto& compManager = _world->GetComponentManager();
//      auto id = entManager.CreateEntity().GetID();
//
//      compManager.AddComponent<Transform>(id);
//      compManager.AddComponent<IsActive>(id);
//      compManager.AddComponent<Sprite>(id);
//      compManager.AddComponent<RigidBody>(id);
//      compManager.AddComponent<Alphabet>(id);
//
//      auto& sprite = compManager.GetComponent<Sprite>(id).get();
//      auto& Get = compManager.GetComponent<Alphabet>(id).get();
//      auto& AlphabetTransform = compManager.GetComponent<Transform>(id).get();
//
//      Get._run = true;
//
//      sprite.shaderRef = shaderpgms.find("shdrpgm");
//      sprite.modelRef = models.find("square");
//      sprite.isQuad = true;
//      sprite.hasTexture = true;
//      sprite.textureRef = textures.find("dog");
//      sprite.layerNum = 40;
//
//      Get.health = (float)duration; //lifespan
//      sprite.textureIdx.x = (float)(characters[index] - 65);
//      sprite.textureIdx.y = 1.0f;
//
//      AlphabetTransform.scale = scale;
//      AlphabetTransform.position.x = pos.x + (scale.x * index);
//      AlphabetTransform.position.y = pos.y;
//    }
//    index--;
//  }
//
//}

void Graphics::TESTRENDER()
{
  DrawBuffers(Graphics::framebuffer);
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

void Graphics::DrawBuffers(unsigned int framebuffer)
{
  //SortLayers();

  if (mouseEditor)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  }

  //for (auto& pair : lookup)
  for (auto const& pair : Graphics::sprites)
  {
    // Part 1: Install the shader program used by this object to render its model using GLSLShader::Use()
    pair.second->shaderRef->second.Use();

    // Part 2: Bind the object's VAO handle using glBindVertexArray
    glBindVertexArray(pair.second->modelRef->second->GetVaoID());

    glEnable(GL_BLEND);

    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);
    //glCullFace(GL_BACK);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear framebuffer (color and depth)

    if (pair.second->modelRef == Graphics::FindModel("sphere") ||
      pair.second->modelRef == Graphics::FindModel("cube") ||
      pair.second->modelRef == Graphics::FindModel("cone") ||
      pair.second->modelRef == Graphics::FindModel("torus") ||
      pair.second->modelRef == Graphics::FindModel("pyramid") ||
      pair.second->modelRef == Graphics::FindModel("cylinder"))
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDisable(GL_CULL_FACE);
      glEnable(GL_LINE_SMOOTH);
    }
    else
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glEnable(GL_CULL_FACE);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (pair.second->hasTexture)
    {
      if (pair.second->textureRef != Graphics::textures.end())
      {
        glBindTextureUnit(1, pair.second->textureRef->second.GetHandle());

        glEnable(GL_BLEND);

        glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(pair.second->textureRef->second.GetHandle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
      }
    }

    CheckUniformLoc(*(pair.second));

    // Part 4: Render OpenGL primitives encapsulated by this object's VAO using glDrawElements
    if (pair.second->modelRef->second->GetPrimitiveType() != GL_LINES)
    {
      glDrawElements(pair.second->modelRef->second->GetPrimitiveType(),
        pair.second->modelRef->second->GetDrawCount(), GL_UNSIGNED_SHORT, NULL);
    }
    else
    {
      //glLineWidth() is actually a deprecated feature, so highly likely not to
      // work on a newer core version of OpenGL
      glLineWidth(5.0f);
      glDrawArrays(pair.second->modelRef->second->GetPrimitiveType(), 0, 2);
    }

    // Part 5: Clean up
    glBindVertexArray(0);
    pair.second->shaderRef->second.UnUse();
  }
}

void Graphics::CheckUniformLoc(Sprite& sprite)
{

  for (auto const& entity : mEntities)
  {
    Camera& oi = engine->world.GetComponent<Camera>(entity);

    std::cout << "camera" << oi.prevPos.x << std::endl;
  }

  //unsigned int currCamID = 0;

  //auto& manager = _world->GetComponentManager();
  //auto& _camera = manager.GetComponent<Camera>(currCamID).get();

  //GLint uniform_var_loc1 =
  //  glGetUniformLocation(sprite.shaderRef->second.GetHandle(),
  //    "uModelToNDC");

  //GLint uniform_var_loc2 =
  //  glGetUniformLocation(sprite.shaderRef->second.GetHandle(),
  //    "uColor");

  //GLint uniform_var_loc3 =
  //  glGetUniformLocation(sprite.shaderRef->second.GetHandle(),
  //    "uTextureCheck");

  //GLint uniform_var_loc4 =
  //  glGetUniformLocation(sprite.shaderRef->second.GetHandle(),
  //    "TextureIndex");

  //GLint uniform_var_loc5 =
  //  glGetUniformLocation(sprite.shaderRef->second.GetHandle(),
  //    "TextureDimensions");

  //GLint uniform_var_loc6 =
  //  glGetUniformLocation(sprite.shaderRef->second.GetHandle(),
  //    "LightTimer");

  //GLint uniform_var_loc7 =
  //  glGetUniformLocation(sprite.shaderRef->second.GetHandle(),
  //    "ShakeTimer");

  //GLint uniform_var_loc8 =
  //  glGetUniformLocation(sprite.shaderRef->second.GetHandle(),
  //    "ShakeScreen");

  //GLuint tex_loc = glGetUniformLocation(sprite.shaderRef->second.GetHandle(), "uTex2d");

  //if (uniform_var_loc1 >= 0)
  //{
  //  if (sprite.modelRef == Graphics::FindModel("sphere") ||
  //    sprite.modelRef == Graphics::FindModel("cube") ||
  //    sprite.modelRef == Graphics::FindModel("cone") ||
  //    sprite.modelRef == Graphics::FindModel("torus") ||
  //    sprite.modelRef == Graphics::FindModel("pyramid") ||
  //    sprite.modelRef == Graphics::FindModel("cylinder"))
  //  {
  //    glm::mat4 mModelNDC;

  //    float eyeRadius = 1.0f * _camera.eyeRadius;
  //    float oneStep = 3.141590118f / 36;
  //    float eyeAlpha = oneStep * _camera.eyeAlpha;
  //    float eyeBeta = oneStep * _camera.eyeBeta;

  //    glm::vec3 view{ eyeRadius * cos(eyeAlpha) * cos(eyeBeta),
  //                    eyeRadius * sin(eyeAlpha),
  //                    eyeRadius * cos(eyeAlpha) * sin(eyeBeta) };

  //    //std::cout << "EyeRadius: " << eyeRadius << std::endl;
  //    //std::cout << "EyeAlpha: " << eyeAlpha << std::endl;
  //    //std::cout << "EyeBeta: " << eyeBeta << std::endl;
  //    //std::cout << "View: {" << view.x << ", " << view.y << ", " << view.z << "} " << std::endl;

  //    glm::vec3 upVec{ 0.0f };
  //    if (_camera.eyeAlpha == 18)
  //    {
  //      glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -oneStep * _camera.eyeBeta, glm::vec3{ 0.0f, 1.0f, 1.0f });

  //      upVec = glm::mat3(rot) * -(glm::vec3{ 1.0f, 0.0f,0.0f });
  //    }
  //    else if (_camera.eyeAlpha == -18)
  //    {
  //      glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -oneStep * _camera.eyeBeta, glm::vec3{ 0.0f, 1.0f, 1.0f });

  //      upVec = glm::mat3(rot) * glm::vec3{ 1.0f, 0.0f,0.0f };
  //    }

  //    if (_camera.eyeAlpha < 18)
  //    {
  //      upVec = glm::vec3{ 0.0f, 1.0f, 0.0f };
  //    }
  //    else
  //    {
  //      upVec = -(glm::vec3{ 0.0f, 1.0f, 0.0f });
  //    }


  //    //glm::mat4 viewMtx = glm::lookAt(view, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
  //    glm::mat4 viewMtx = glm::lookAt(view, glm::vec3{ 0.0f, 0.0f, 0.0f }, upVec);


  //    //glm::mat4 viewMtx = glm::mat4(1.0f);
  //    glm::mat4 projMtx = glm::perspective(glm::radians(90.0f), static_cast<float>(GLHelper::width) / static_cast<float>(GLHelper::height), 1.0f, 500.0f);
  //    //glm::mat4 projMtx = glm::frustum(-0.8f, 0.8f, -0.6f, 0.6f, 1.0f, 80.0f);

  //    glm::mat4 model = glm::mat4(1.0f);
  //    //model = glm::translate(model, glm::vec3{ 0.0f, 0.0f, 10.0f });
  //    //model = glm::scale(model, glm::vec3{ 5.0f, 5.0f, 5.0f });

  //    model = glm::translate(model, spherePos);
  //    model = glm::rotate(model, glm::radians(sphereRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
  //    model = glm::rotate(model, glm::radians(sphereRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
  //    model = glm::rotate(model, glm::radians(sphereRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
  //    model = glm::scale(model, sphereScale);


  //    mModelNDC = projMtx * viewMtx * model;

  //    glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE,
  //      glm::value_ptr(mModelNDC));
  //  }
  //}
  //else
  //{
  //  std::cout << "Uniform variable doesn't exist!!!\n";
  //  std::exit(EXIT_FAILURE);
  //}

  //if (uniform_var_loc2 >= 0)
  //{
  //  //float alpha = 1.0f;
  //  glUniform4f(uniform_var_loc2, sprite.color.x, sprite.color.y, sprite.color.z, sprite.transparency);
  //}
  //else
  //{
  //  std::cout << "Uniform variable doesn't exist!!!\n";
  //  std::exit(EXIT_FAILURE);
  //}

  //if (uniform_var_loc3 >= 0)
  //{
  //  glUniform1i(uniform_var_loc3, sprite.hasTexture);
  //}
  //else
  //{
  //  std::cout << "Uniform variable doesn't exist!!!\n";
  //  std::exit(EXIT_FAILURE);
  //}

  //if (uniform_var_loc4 >= 0)
  //{
  //  glUniform2f(uniform_var_loc4, sprite.textureIdx.x, sprite.textureIdx.y);
  //}
  //else
  //{
  //  std::cout << "Uniform variable doesn't exist!!!\n";
  //  std::exit(EXIT_FAILURE);
  //}

  //if (sprite.hasTexture)
  //{
  //  if (sprite.textureRef != Graphics::textures.end())
  //  {
  //    if (uniform_var_loc5 >= 0)
  //    {
  //      glUniform2f(uniform_var_loc5, sprite.textureRef->second.GetCols(), sprite.textureRef->second.GetRows());
  //    }
  //    else
  //    {
  //      std::cout << "Uniform variable doesn't exist!!!\n";
  //      std::exit(EXIT_FAILURE);
  //    }
  //  }
  //}

  //if (uniform_var_loc7 >= 0)
  //{
  //  glUniform1f(uniform_var_loc7, shakeTimer);
  //}
  //else
  //{
  //  std::cout << "Uniform variable doesn't exist!!!\n";
  //  std::exit(EXIT_FAILURE);
  //}

  //if (uniform_var_loc8 >= 0)
  //{
  //  glUniform1i(uniform_var_loc8, shakeScreen);
  //}
  //else
  //{
  //  std::cout << "Uniform variable doesn't exist!!!\n";
  //  std::exit(EXIT_FAILURE);
  //}

  //if (tex_loc >= 0)
  //{
  //  glUniform1i(tex_loc, 1);
  //}
  //else
  //{
  //  std::cout << "Uniform variable tex_loc doesn't exist!!!\n";
  //  std::exit(EXIT_FAILURE);
  //}
}
