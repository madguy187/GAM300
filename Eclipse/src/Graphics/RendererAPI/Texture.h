#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "../Dep/STB/stb-master/stb_image.h"
#include <iostream>

namespace Eclipse
{

#ifndef CURRENT_CODE
    class Texture 
    {
    public:
        Texture(std::string dir, std::string path, aiTextureType type);

        void generate();
        void load(bool flip = true);

        void bind();

        // texture object
        unsigned int id;
        aiTextureType type;
        std::string dir;
        std::string path;
    };
#else
    class Texture
    {
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
#endif

}

#endif /* TEXTURE_H */