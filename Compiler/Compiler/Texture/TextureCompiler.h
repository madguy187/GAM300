#pragma once
#include "Compilers.h"
#include "AssimpLoader.h"
#include <string>

namespace EclipseCompiler
{
    class TextureCompiler : public Helper, public ICompiler
    {
    public:
        // I will use textures that are not assigned first to use index more than 100
        std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>> Textures;
        std::ofstream TextureFileWrite;
        std::fstream TextureFileRead;

        // Texture Container
        std::unordered_map<std::string, Texture> TextureCotainer;

    private:
        void ReadFile();
        void WriteToFile(std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>& LoadedTextures);

    public:
        void LoadFile(const std::string& modelFile) override;
        void Init() override;
        void ReleaseFile() override;
        void ReadFile(std::string& in) override;
    };
}