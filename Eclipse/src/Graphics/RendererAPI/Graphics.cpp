#define _USE_MATH_DEFINES

#include "pch.h"
#include "Graphics.h"
#include "type_ptr.hpp"
#include "matrix_transform_2d.hpp"
#include <algorithm>


/*                                                   objects with file scope
----------------------------------------------------------------------------- */
using namespace rapidjson;
using namespace Eclipse;

std::unordered_map<std::string, Shader> Graphics::shaderpgms;
std::unordered_map<std::string, std::unique_ptr<IModel>> Graphics::models;
std::multimap<std::string, Texture> Graphics::textures;

typedef std::multimap<std::string, Texture>::iterator MMAPIterator;

void Graphics::load()
{
    ////Loads all the shader programs listed in the shader file
    LoadShaders("src//Assets//Shaders");

    //Loads all models listed
    LoadModels();

    ENGINE_CORE_INFO("Loaded Models , Shaders and Textures Successful");
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
}

void Graphics::LoadShaders(std::string shaderFile)
{
    for (auto& dirEntry : std::filesystem::directory_iterator(shaderFile))
    {
        const auto& path = dirEntry.path();
        auto relativePath = relative(path, "src//");
        std::string FolderName = relativePath.filename().string();
        std::string PathName = ("src/Assets/Shaders/" + FolderName).c_str();

        for (auto& dirEntry_ : std::filesystem::directory_iterator(PathName))
        {
            const auto& path_ = dirEntry_.path();
            auto relativePath_ = relative(path_, "src//");
            std::string ShaderFragOrVert = relativePath_.filename().string();
            std::string ShaderFragConcatenate = "src/Assets/Shaders/" + FolderName + "/" + ShaderFragOrVert;
            engine->GraphicsManager.ShaderMap[FolderName].push_back(ShaderFragConcatenate);
        }

        if (engine->GraphicsManager.ShaderMap[FolderName].size() > 2)
        {
            initShaderpgms(true, FolderName, engine->GraphicsManager.ShaderMap[FolderName][2],
                engine->GraphicsManager.ShaderMap[FolderName][0], engine->GraphicsManager.ShaderMap[FolderName][1]);
        }
        else
        {
            initShaderpgms(false, FolderName, engine->GraphicsManager.ShaderMap[FolderName][1],
                engine->GraphicsManager.ShaderMap[FolderName][0]);
        }
    }
}

void Graphics::LoadModels()
{
    auto ModelName = engine->GraphicsManager.GetModelName(13);
    models.emplace(ModelName, ModelFactory::create(13));

    engine->AssimpManager.InsertPrimitiveName("BS");
    engine->AssimpManager.InsertPrimitiveName("BoundingSphere");

    for (unsigned int i = 0; i < static_cast<unsigned int>(LoadingModels::MAXCOUNT); i++)
    {
        auto ModelName_ = engine->GraphicsManager.GetModelName(i);
        models.emplace(ModelName_, ModelFactory::create(i));

        engine->AssimpManager.InsertPrimitiveName(ModelName_);
    }
}

void Eclipse::Graphics::ThreadLoadShaders()
{
    //Loads all the shader programs listed in the shader file
    LoadShaders("src//Assets//Shaders");
}

/******************************************************************************/
/*!
    This function sets up the shader program by loading the shader files and
    uses the shader program pipeline functionality implemented in class
    GLSLShader.
 */
 /******************************************************************************/
void Graphics::initShaderpgms(bool in, std::string shdrpgm_name,
    std::string vtx_shdr, std::string frg_shdr, std::string geo)
{
    std::vector<std::pair<GLenum, std::string>> shdr_files;

    if (in == true)
    {
        shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vtx_shdr));
        shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frg_shdr));
        shdr_files.push_back(std::make_pair(GL_GEOMETRY_SHADER, geo));
    }
    else
    {
        shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vtx_shdr));
        shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frg_shdr));
    }

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

Texture Eclipse::Graphics::FindTextures(std::string in)
{
    std::pair<MMAPIterator, MMAPIterator> result = Graphics::textures.equal_range(in);

    if (textures.find(in) == textures.end())
    {
        ENGINE_LOG_ASSERT(false, "texture cannot be found.");
        return textures.begin()->second;
    }
    else
    {
        MMAPIterator it = result.first;
        return it->second;
    }
}

void Eclipse::Graphics::GetTexuresForModels(std::string in, MaterialComponent com)
{
    std::pair<MMAPIterator, MMAPIterator> result = textures.equal_range(in);

    for (MMAPIterator it = result.first; it != result.second; it++)
    {
        com.HoldingTextures.push_back(it->second);
    }
}
