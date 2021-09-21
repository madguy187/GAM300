#include "pch.h"
#include "Texture.h"

using namespace Eclipse;

Texture::Texture(std::string dir, std::string path, aiTextureType type) :
    Directory(dir),
    Path(path),
    Type(type)
{

}

void Texture::Generate()
{
    glGenTextures(1, &handle);
}

void Texture::Load(bool flip)
{
    stbi_set_flip_vertically_on_load(flip);

    int width, height, nChannels;

    unsigned char* data = stbi_load((Directory + "/" + Path).c_str(), &width, &height, &nChannels, 0);

    GLenum colorMode = GL_RGB;

    switch (nChannels)
    {
    case 1:
        colorMode = GL_RED;
        break;
    case 4:
        colorMode = GL_RGBA;
        break;
    };

    if (data)
    {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Image not loaded at " << Path << std::endl;
    }

    stbi_image_free(data);
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, handle);
}

void Eclipse::Texture::SetDirectory(std::string dir)
{
    Directory = dir;
}

void Eclipse::Texture::SetPath(std::string pathhh)
{
    Path = pathhh;
}

void Eclipse::Texture::SetType(aiTextureType in)
{
    Type = in;
}

std::string Eclipse::Texture::GetDirectory()
{
    return Directory;
}

std::string Eclipse::Texture::GetPath()
{
    return Path;
}

aiTextureType Eclipse::Texture::GetType()
{
    return Type;
}

unsigned int Eclipse::Texture::GetId()
{
    return handle;
}

Texture::Texture(std::string pathname) :
    sheetWidth{ 0 }, sheetHeight{ 0 }, numCols{ 0 }, numRows{ 0 },
    channels{ 0 }
{
    initTexture(pathname);
}

void Texture::initTexture(std::string pathname)
{
    unsigned char* data = stbi_load(pathname.c_str(), &sheetWidth, &sheetHeight, &channels, STBI_rgb_alpha);

    if (data)
    {
        GLuint texobj_hdl;

        //// define and initialize a handle to texture object that will encapsulate two-dimensional textures
        glCreateTextures(GL_TEXTURE_2D, 1, &texobj_hdl);

        //// allocate GPU storage for texture image data loaded from file
        glTextureStorage2D(texobj_hdl, 1, GL_RGBA8, sheetWidth, sheetHeight);

        //// copy image data from client memory to GPU texture buffer memory
        glTextureSubImage2D(texobj_hdl, 0, 0, 0, sheetWidth, sheetHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        //// client memory not required since image is buffered in GPU memory
        stbi_image_free(data);

        handle = texobj_hdl;
    }
    else
    {
        std::string error = stbi_failure_reason();
        std::cout << "Texture could not be loaded! : " << error << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

GLuint Texture::GetHandle()
{
    return handle;
}

GLint Texture::GetWidth()
{
    return sheetWidth;
}

GLint Texture::GetHeight()
{
    return sheetHeight;
}

GLint Texture::GetChannels()
{
    return channels;
}

GLfloat Texture::GetCols()
{
    return static_cast<float>(numCols);
}

GLfloat Texture::GetRows()
{
    return static_cast<float>(numRows);
}

GLint Texture::GetSpriteWidth()
{
    return spriteWidth;
}

GLint Texture::GetSpriteHeight()
{
    return spriteHeight;
}

void Texture::DeleteTexture()
{
    glDeleteTextures(1, &handle);
}

void Texture::setSpriteWidth(GLint _spriteWidth)
{
    spriteWidth = static_cast<int>(_spriteWidth);
    numCols = sheetWidth / _spriteWidth;
}

void Texture::setSpriteHeight(GLint _spriteHeight)
{
    spriteHeight = static_cast<int>(_spriteHeight);
    numRows = sheetHeight / _spriteHeight;
}