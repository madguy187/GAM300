/**********************************************************************************
* \file            Font.h
*
* \brief        Definition of the Font class function(s).
*
* \author       Lin Yanping Rachel
*
* \email		l.yanpingrachel@digipen.edu
*
* \date			20 sep 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef FONT_H
#define FONT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <stb_image.h>
#include <vector>
#include <iostream>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

class Fonts
{
    struct Character
    {
        unsigned int TextureID;
        glm::vec2   Size;
        unsigned int Advance;
        glm::vec2   Bearing;
        glm::vec2 startUV;
        glm::vec2 endUV;
    };
public:
    Fonts(std::string pathname);

    void initFont(std::string pathname);
    void LoadCharacters();
    void CreateBuffers();

    GLuint GetVAO();
    GLuint GetVBO();

    std::map<unsigned char, Character> Characters;
private:
    GLuint vaoID;
    GLuint vboID;

    FT_Face face;
    FT_Library ft;

    std::vector<glm::vec2> PosVec;
    std::vector<glm::vec2> TextVec;
};

#endif /* FONT_H */