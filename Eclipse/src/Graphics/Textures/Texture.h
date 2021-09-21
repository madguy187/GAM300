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
    public:


        // 3D Textures =============================
    private:
        //unsigned int Id;
        aiTextureType Type;
        std::string Directory;
        std::string Path;

    public:
        Texture() {};
        Texture(std::string dir, std::string path, aiTextureType type);

        void Generate();
        void Load(bool flip = true);
        void Bind();
        void SetDirectory(std::string dir);
        void SetPath(std::string pathhh);
        void SetType(aiTextureType in);
        std::string GetDirectory();
        std::string GetPath();
        aiTextureType GetType();
        unsigned int GetId();


        // 2D Textures =============================
    public:
        Texture(std::string pathname);
        void initTexture(std::string pathname);

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
        GLuint handle;
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