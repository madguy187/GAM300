#include "pch.h"
#include "Texture.h"

#define TINYDDSLOADER_IMPLEMENTATION
#include "../Dep/tinyddsloader/tinyddsloader.h"

#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#define GL_COMPRESSED_RED_RGTC1_EXT 0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1_EXT 0x8DBC
#define GL_COMPRESSED_RED_GREEN_RGTC2_EXT 0x8DBD
#define GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT 0x8DBE
#endif /* GL_EXT_texture_compression_s3tc */

using namespace Eclipse;

struct GLSwizzle 
{
    GLenum m_r, m_g, m_b, m_a;
};

struct GLFormat 
{
    tinyddsloader::DDSFile::DXGIFormat m_dxgiFormat;
    GLenum m_type;
    GLenum m_format;
    GLSwizzle m_swizzle;
};

bool TranslateFormat(tinyddsloader::DDSFile::DXGIFormat fmt, GLFormat* outFormat) {
    static const GLSwizzle sws[] = 
    {
        {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA},
        {GL_BLUE, GL_GREEN, GL_RED, GL_ALPHA},
        {GL_BLUE, GL_GREEN, GL_RED, GL_ONE},
        {GL_RED, GL_GREEN, GL_BLUE, GL_ONE},
        {GL_RED, GL_ZERO, GL_ZERO, GL_ZERO},
        {GL_RED, GL_GREEN, GL_ZERO, GL_ZERO},
    };

    using DXGIFmt = tinyddsloader::DDSFile::DXGIFormat;
    static const GLFormat formats[] = 
    {
        {DXGIFmt::R8G8B8A8_UNorm, GL_UNSIGNED_BYTE, GL_RGBA, sws[0]},
        {DXGIFmt::B8G8R8A8_UNorm, GL_UNSIGNED_BYTE, GL_RGBA, sws[1]},
        {DXGIFmt::B8G8R8X8_UNorm, GL_UNSIGNED_BYTE, GL_RGBA, sws[2]},
        {DXGIFmt::BC1_UNorm, 0, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, sws[0]},
        {DXGIFmt::BC2_UNorm, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, sws[0]},
        {DXGIFmt::BC3_UNorm, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, sws[0]},
        {DXGIFmt::BC4_UNorm, 0, GL_COMPRESSED_RED_RGTC1_EXT, sws[0]},
        {DXGIFmt::BC4_SNorm, 0, GL_COMPRESSED_SIGNED_RED_RGTC1_EXT, sws[0]},
        {DXGIFmt::BC5_UNorm, 0, GL_COMPRESSED_RED_GREEN_RGTC2_EXT, sws[0]},
        {DXGIFmt::BC5_SNorm, 0, GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT,
         sws[0]},
    };

    for (const auto& format : formats) 
    {
        if (format.m_dxgiFormat == fmt) 
        {
            if (outFormat) 
            {
                *outFormat = format;
            }

            return true;
        }
    }

    return false;
}

bool IsCompressed(GLenum fmt) 
{
    switch (fmt) 
    {
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
    case GL_COMPRESSED_RED_RGTC1_EXT:
    case GL_COMPRESSED_SIGNED_RED_RGTC1_EXT:
    case GL_COMPRESSED_RED_GREEN_RGTC2_EXT:
    case GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT:
        return true;
    default:
        return false;
    }
}

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
    std::filesystem::path textureFile{ Directory + "/" + Path };

    if (textureFile.extension().compare(".dds") == 0)
    {
       // std::cout << "Loading compressed texture.." << std::endl;
        //std::cout << std::endl;

        LoadCompressedTextures(flip);
    }
    else
    {
        //std::cout << "Loading uncompressed texture.." << std::endl;
        //std::cout << std::endl;

        LoadUncompressedTextures(flip);
    }
}

void Eclipse::Texture::LoadUncompressedTextures(bool flip)
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

bool Eclipse::Texture::LoadCompressedTextures(bool flip)
{
    using namespace tinyddsloader;

    DDSFile texture;
    auto result = texture.Load((Directory + "/" + Path).c_str());

    assert(result == tinyddsloader::Result::Success);

    if ((texture.GetTextureDimension() == DDSFile::TextureDimension::Unknown) ||
        (texture.GetTextureDimension() == DDSFile::TextureDimension::Texture3D))
    {
        std::cerr << "Unsupported texture dimensions!" << std::endl;
        return false;
    }

    GLenum type = GL_INVALID_ENUM;
    bool isArray = false;

    if (texture.GetArraySize() > 1)
    {
        if (texture.IsCubemap())
        {
            if (texture.GetArraySize() > 6)
            {
                type = GL_TEXTURE_CUBE_MAP_ARRAY;
                isArray = true;
            }
            else
            {
                type = GL_TEXTURE_CUBE_MAP;
            }
        }
        else
        {
            type = GL_TEXTURE_2D_ARRAY;
            isArray = true;
        }
    }
    else
    {
        type = GL_TEXTURE_2D;
    }

    GLFormat format;
    if (!TranslateFormat(texture.GetFormat(), &format))
    {
        std::cout << "Failed to translate format." << std::endl;
        return false;
    }

    glGenTextures(1, &handle);
    glBindTexture(type, handle);
    glTexParameteri(type, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(type, GL_TEXTURE_MAX_LEVEL, texture.GetMipCount() - 1);
    glTexParameteri(type, GL_TEXTURE_SWIZZLE_R, format.m_swizzle.m_r);
    glTexParameteri(type, GL_TEXTURE_SWIZZLE_G, format.m_swizzle.m_g);
    glTexParameteri(type, GL_TEXTURE_SWIZZLE_B, format.m_swizzle.m_b);
    glTexParameteri(type, GL_TEXTURE_SWIZZLE_A, format.m_swizzle.m_a);

    switch (type)
    {
    case GL_TEXTURE_2D:
        glTexStorage2D(type, texture.GetMipCount(), format.m_format,
            texture.GetWidth(), texture.GetHeight());
        break;
    case GL_TEXTURE_CUBE_MAP:
        glTexStorage2D(type, texture.GetMipCount(), format.m_format,
            texture.GetWidth(), texture.GetHeight());
        break;
    case GL_TEXTURE_2D_ARRAY:
        glTexStorage3D(type, texture.GetMipCount(), format.m_format,
            texture.GetWidth(), texture.GetHeight(), texture.GetArraySize());
        break;
    case GL_TEXTURE_CUBE_MAP_ARRAY:
        glTexStorage3D(type, texture.GetMipCount(), format.m_format,
            texture.GetWidth(), texture.GetHeight(), texture.GetArraySize());
        break;
    default:
        glBindTexture(type, 0);
        return false;
    }

    if (flip)
    {
        texture.Flip();
    }

    uint32_t numFaces = texture.IsCubemap() ? 6 : 1;
    for (uint32_t layer = 0; layer < texture.GetArraySize(); layer++)
    {
        for (uint32_t face = 0; face < numFaces; face++)
        {
            for (uint32_t level = 0; level < texture.GetMipCount(); level++)
            {
                GLenum type2 = texture.IsCubemap()
                    ? (GL_TEXTURE_CUBE_MAP_POSITIVE_X + face)
                    : type;
                auto imageData = texture.GetImageData(level, layer * numFaces);
                switch (type)
                {
                case GL_TEXTURE_2D:
                case GL_TEXTURE_CUBE_MAP:
                {
                    auto w = imageData->m_width;
                    auto h = isArray ? layer : imageData->m_height;

                    if (IsCompressed(format.m_format))
                    {
                        glCompressedTexSubImage2D(
                            type2, level, 0, 0, w, h, format.m_format,
                            imageData->m_memSlicePitch, imageData->m_mem);
                    }
                    else
                    {
                        glTexSubImage2D(type2, level, 0, 0, w, h,
                            format.m_format, format.m_type,
                            imageData->m_mem);
                    }
                    break;
                }
                default:
                    glBindTexture(type, 0);
                    return false;
                }
            }
        }
    }

    glBindTexture(type, 0);

    return true;

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

unsigned int Eclipse::Texture::GetType()
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
    //std::cout << "Path: " << pathname << std::endl;
    std::filesystem::path textureFile{ pathname };

    if (textureFile.extension().compare(".dds") == 0)
    {
        //std::cout << "Loading compressed texture.." << std::endl;
        //std::cout << std::endl;

        LoadCompressedTextures(pathname);
    }
    else
    {
        //std::cout << "Loading uncompressed texture.." << std::endl;
        //std::cout << std::endl;

        LoadUncompressedTextures(pathname);
    }
}

void Eclipse::Texture::LoadUncompressedTextures(std::string pathname)
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

bool Eclipse::Texture::LoadCompressedTextures(std::string pathname)
{
    using namespace tinyddsloader;

    DDSFile texture;
    auto result = texture.Load((pathname).c_str());

    assert(result == tinyddsloader::Result::Success);

    if ((texture.GetTextureDimension() == DDSFile::TextureDimension::Unknown) ||
        (texture.GetTextureDimension() == DDSFile::TextureDimension::Texture3D))
    {
        std::cerr << "Unsupported texture dimensions!" << std::endl;
        return false;
    }

    GLenum type = GL_INVALID_ENUM;
    bool isArray = false;

    if (texture.GetArraySize() > 1)
    {
        if (texture.IsCubemap())
        {
            if (texture.GetArraySize() > 6)
            {
                type = GL_TEXTURE_CUBE_MAP_ARRAY;
                isArray = true;
            }
            else
            {
                type = GL_TEXTURE_CUBE_MAP;
            }
        }
        else
        {
            type = GL_TEXTURE_2D_ARRAY;
            isArray = true;
        }
    }
    else
    {
        type = GL_TEXTURE_2D;
    }

    GLFormat format;
    if (!TranslateFormat(texture.GetFormat(), &format))
    {
        std::cout << "Failed to translate format." << std::endl;
        return false;
    }

    glGenTextures(1, &handle);
    glBindTexture(type, handle);
    glTexParameteri(type, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(type, GL_TEXTURE_MAX_LEVEL, texture.GetMipCount() - 1);
    glTexParameteri(type, GL_TEXTURE_SWIZZLE_R, format.m_swizzle.m_r);
    glTexParameteri(type, GL_TEXTURE_SWIZZLE_G, format.m_swizzle.m_g);
    glTexParameteri(type, GL_TEXTURE_SWIZZLE_B, format.m_swizzle.m_b);
    glTexParameteri(type, GL_TEXTURE_SWIZZLE_A, format.m_swizzle.m_a);

    switch (type) 
    {
    case GL_TEXTURE_2D:
        glTexStorage2D(type, texture.GetMipCount(), format.m_format,
            texture.GetWidth(), texture.GetHeight());
        break;
    case GL_TEXTURE_CUBE_MAP:
        glTexStorage2D(type, texture.GetMipCount(), format.m_format,
            texture.GetWidth(), texture.GetHeight());
        break;
    case GL_TEXTURE_2D_ARRAY:
        glTexStorage3D(type, texture.GetMipCount(), format.m_format,
            texture.GetWidth(), texture.GetHeight(), texture.GetArraySize());
        break;
    case GL_TEXTURE_CUBE_MAP_ARRAY:
        glTexStorage3D(type, texture.GetMipCount(), format.m_format,
            texture.GetWidth(), texture.GetHeight(), texture.GetArraySize());
        break;
    default:
        glBindTexture(type, 0);
        return false;
    }

    uint32_t numFaces = texture.IsCubemap() ? 6 : 1;
    for (uint32_t layer = 0; layer < texture.GetArraySize(); layer++) 
    {
        for (uint32_t face = 0; face < numFaces; face++) 
        {
            for (uint32_t level = 0; level < texture.GetMipCount(); level++) 
            {
                GLenum type2 = texture.IsCubemap()
                    ? (GL_TEXTURE_CUBE_MAP_POSITIVE_X + face)
                    : type;
                auto imageData = texture.GetImageData(level, layer * numFaces);
                switch (type) 
                {
                case GL_TEXTURE_2D:
                case GL_TEXTURE_CUBE_MAP: 
                {
                    auto w = imageData->m_width;
                    auto h = isArray ? layer : imageData->m_height;

                    if (IsCompressed(format.m_format)) 
                    {
                        glCompressedTexSubImage2D(
                            type2, level, 0, 0, w, h, format.m_format,
                            imageData->m_memSlicePitch, imageData->m_mem);
                    }
                    else 
                    {
                        glTexSubImage2D(type2, level, 0, 0, w, h,
                            format.m_format, format.m_type,
                            imageData->m_mem);
                    }
                    break;
                }
                default:
                    glBindTexture(type, 0);
                    return false;
                }
            }
        }
    }

    glBindTexture(type, 0);

    return true;
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