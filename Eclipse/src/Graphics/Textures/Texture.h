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
        void SetDirectory(std::string dir);
        void SetPath(std::string pathhh);
        void SetType(aiTextureType in);
        std::string GetDirectory();
        std::string GetPath();
        unsigned int GetType();
        unsigned int GetId();

    private:
        //unsigned int Id;

        // 2D Textures =============================
    public:
        /// Public for now - from Jian Herng.
        unsigned int Type; //aiTextureType
        std::string Directory;
        unsigned int handle = 0;
        ///
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
        GLint sheetWidth = 0;
        GLint sheetHeight = 0;
        GLint channels = 0;
        GLint spriteWidth = 0;
        GLint spriteHeight = 0;
        GLint numCols = 0;
        GLint numRows = 0;
    };
}

#endif /* TEXTURE_H */