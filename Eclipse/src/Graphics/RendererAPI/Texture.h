/**********************************************************************************
* \file            Texture.h
*
* \brief        Definition of the Texture class function(s).
*
* \author       Lin Yanping Rachel
*
* \email		l.yanpingrachel@digipen.edu
*
* \date			20 aug 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H

//#include <GL/glew.h>
//#include <glm/glm.hpp>
#include <string>
#include <stb_image.h>
#include <iostream>

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

#endif /* TEXTURE_H */