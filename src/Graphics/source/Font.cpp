#include "pch.h"
#include "../src/Graphics/include/Font.h"

Fonts::Fonts(std::string pathname)
{
    initFont(pathname);
    LoadCharacters();
    CreateBuffers();
}

void Fonts::initFont(std::string pathname)
{
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (FT_New_Face(ft, pathname.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Fonts::LoadCharacters()
{
    //set size to load glyphs
    FT_Set_Pixel_Sizes(face, 0, 48);

    //disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int textureWidth = 0;
    int textureHeight = 0;

    //load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        textureWidth += face->glyph->bitmap.width;

        if (textureHeight < static_cast<int>(face->glyph->bitmap.rows))
        {
            textureHeight = static_cast<int>(face->glyph->bitmap.rows);
        }

        Character character;
        character.TextureID = texture;
        character.Size = glm::vec2{ static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows) };
        character.Bearing = glm::vec2{ static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top) };
        character.Advance = static_cast<unsigned int>(face->glyph->advance.x);

        //// set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Characters.insert(std::make_pair(c, character));
    }

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        textureWidth,
        textureHeight,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        NULL
    );

    int UJumps = 0;
    int VJumps = 0;

    for (GLubyte c = 0; c < 128; ++c)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        float uS = static_cast<float>(UJumps) / static_cast<float>(textureWidth);
        float vS = static_cast<float>(VJumps) / static_cast<float>(textureHeight);

        float uE = static_cast<float>(UJumps + static_cast<int>(face->glyph->bitmap.width));
        uE = uE / static_cast<float>(textureWidth);

        float vE = static_cast<float>(VJumps + face->glyph->bitmap.rows);
        vE = vE / static_cast<float>(textureHeight);

        Characters[c].startUV = glm::vec2{ uS, vS };
        Characters[c].endUV = glm::vec2{ uE, vE };

        if (face->glyph->bitmap.width != 0)
        {
            glTexSubImage2D(
                GL_TEXTURE_2D,
                0,
                UJumps,
                VJumps,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
        }

        UJumps += face->glyph->bitmap.width;

    }

    glBindTexture(GL_TEXTURE_2D, 0);

    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Fonts::CreateBuffers()
{
    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboID);
    glBindVertexArray(vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4 * 128, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLuint Fonts::GetVAO()
{
    return vaoID;
}

GLuint Fonts::GetVBO()
{
    return vboID;
}

