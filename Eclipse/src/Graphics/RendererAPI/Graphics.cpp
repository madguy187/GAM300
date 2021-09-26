#define _USE_MATH_DEFINES

#include "pch.h"
#include "Graphics.h"
#include "type_ptr.hpp"
#include "matrix_transform_2d.hpp"

/*                                                   objects with file scope
----------------------------------------------------------------------------- */
using namespace rapidjson;
using namespace Eclipse;

std::unordered_map<std::string, Shader> Graphics::shaderpgms;
std::unordered_map<std::string, std::unique_ptr<IModel>> Graphics::models;
std::unordered_map<std::string, Texture> Graphics::textures;
std::multimap<unsigned int, MeshComponent*> Graphics::sprites;
std::set<unsigned int> Graphics::sortedID;

void Graphics::load()
{
    //Loads all the shader programs listed in the shader file
    LoadShaders("src//Assets//Shaders");

    //Loads all models listed
    LoadModels();

    //Loads all textures listed
    LoadTextures("src/Assets/Textures/Textures.json");

    ENGINE_CORE_INFO("Loaded Models , Shaders and Textures Successful");
}

void Graphics::unload()
{
    for (auto& it = shaderpgms.begin(); it != shaderpgms.end(); ++it)
    {
        it->second.DeleteShaderProgram();
    }

    for (auto& it = models.begin(); it != models.end(); ++it)
    {
        it->second->DeleteModel();
    }

    for (auto& fb : OpenGL_Context::_Framebuffers)
    {
        delete fb.second;
    }
}

void Graphics::LoadShaders(std::string shaderFile)
{
    for (auto& dirEntry : std::filesystem::directory_iterator(shaderFile))
    {
        const auto& path = dirEntry.path();
        auto relativePath = relative(path, "src//");
        std::string FolderName = relativePath.filename().string();
        std::string PathName = ("src/Assets/Shaders/" + FolderName).c_str();

        for (auto& dirEntry : std::filesystem::directory_iterator(PathName))
        {
            const auto& path = dirEntry.path();
            auto relativePath = relative(path, "src//");
            std::string ShaderFragOrVert = relativePath.filename().string();
            std::string ShaderFragConcatenate = "src/Assets/Shaders/" + FolderName + "/" + ShaderFragOrVert;
            engine->GraphicsManager.ShaderMap[FolderName].push_back(ShaderFragConcatenate);
        }

        initShaderpgms(FolderName, engine->GraphicsManager.ShaderMap[FolderName][1],
            engine->GraphicsManager.ShaderMap[FolderName][0]);
    }
}

void Graphics::LoadModels()
{
    for (unsigned int i = 0; i < static_cast<unsigned int>(LoadingModels::MAXCOUNT); i++)
    {
        auto& ModelName = engine->GraphicsManager.GetModelName(i);
        models.emplace(ModelName, ModelFactory::create(i));

        engine->AssimpManager.AllPrimitiveModelsNames.push_back(ModelName);
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
    int clrChannels = 0;
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
