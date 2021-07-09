#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include <vector>
#include "shader.h"
#include "Graphics/ModelFactory/Sky/glmemory.hpp"

namespace Eclipse
{
    class Cubemap
    {
    public:
        Cubemap();

        void loadTextures(std::string dir,
            std::string right = "right.png",
            std::string left = "left.png",
            std::string top = "top.png",
            std::string bottom = "bottom.png",
            std::string front = "front.png",
            std::string back = "back.png");

        void init();
        void render(Shader& shader);
        void cleanup();

    private:

        // texture object
        unsigned int id;
        std::string dir;
        std::vector<std::string> faces;
        bool hasTextures;

        // cube object
        GLuint vboID;
        //ArrayObject VAO;

        unsigned int NEWVAO;
        unsigned int VBO, EBO;
    };
}

#endif