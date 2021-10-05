#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include <vector>
#include "shader.h"

namespace Eclipse
{
    class CubeMap
    {
    private:
        unsigned int Id;
        std::string Directory;
        std::vector<std::string> Faces;
        bool HasTextures;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        bool Visible = true;

    public:
        CubeMap();
        void CreateSky(std::string _dir);
        void Render(Shader& shader);
        void Cleanup();
        std::string GetDirectory();
        void SetDirectory(std::string in);
        unsigned int GetVaoID();
        unsigned int GetVboID();
        unsigned int GetEboID();
        void SetTextureFlag(bool in);
        bool GetTextureFlag();
        size_t GetFaceCount();
        void SetVisible(bool in);
        bool GetVisibllity();

    private:
        void Setup(int& height, int& width, int& channel);

        void LoadTextures(std::string dir,
            std::string right = "right.png", std::string left = "left.png", std::string top = "top.png",
            std::string bottom = "bottom.png", std::string front = "front.png", std::string back = "back.png");
        void Load(std::string _dir, std::string right, std::string left,
            std::string top, std::string bottom, std::string front, std::string back);
    };
}

#endif