#pragma once
#include "Compilers.h"
#include "AssimpLoader.h"
#include <string>

namespace EclipseCompiler
{
    class TextureCompiler : public Compilers
    {
    public:
        std::unordered_map<std::string, std::unique_ptr<Texture>> Textures;
        std::ofstream GeometryFile;
        std::fstream GeometryFileWrite;
        std::vector<Texture> TextureContainer;

    private:
        void LoadFile(const std::string& modelFile);

    public:
        void Init();
        void ReleaseFile(std::string& in);
    };
}