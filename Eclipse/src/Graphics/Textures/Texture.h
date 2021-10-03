#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "../Dep/STB/stb-master/stb_image.h"
#include <iostream>

namespace Eclipse
{
    class Texture
    {
        // Must haev the ability to check type of textures next
        // will need to redo some parts as i need to load in fbx textures seperately

        // 3D Textures =============================
    public:
        Texture() {};
        Texture(std::string dir, std::string path, aiTextureType type);

        void Generate();
        void Load(bool flip = true);
        void LoadUncompressedTextures(bool flip);
        bool LoadCompressedTextures(bool flip);
        void Bind();
        void BindModel();
        void SetDirectory(std::string dir);
        void SetPath(std::string pathhh);
        void SetType(aiTextureType in);
        std::string GetDirectory();
        std::string GetPath();
        aiTextureType GetType();
        unsigned int GetId();

    private:
        //unsigned int Id;
        aiTextureType Type;
        std::string Directory;
        unsigned int id;

        // 2D Textures =============================
    public:
        std::string Path;
        Texture(std::string pathname);
        void initTexture(std::string pathname);
        void LoadUncompressedTextures(std::string pathname);
        bool LoadCompressedTextures(std::string pathname);

        GLuint GetHandle();
        GLint GetWidth();
        GLint GetHeight();
        GLint GetChannels();
        GLfloat GetCols();
        GLfloat GetRows();
        GLint GetSpriteWidth();
        GLint GetSpriteHeight();
        void setSpriteWidth(GLint _spriteWidth);
        void setSpriteHeight(GLint _spriteHeight);
        void DeleteTexture();
    private:
        unsigned int handle;
        GLint sheetWidth;
        GLint sheetHeight;
        GLint channels;
        GLint spriteWidth;
        GLint spriteHeight;
        GLint numCols;
        GLint numRows;
    };
}

#endif /* TEXTURE_H */