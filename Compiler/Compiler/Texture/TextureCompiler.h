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

        // All Other Textures
        std::unordered_map<std::string, Texture> AllOtherTextureCotainer;

    private:
        void ReadFile();
        void WriteToFile(std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>& LoadedTextures);

    public:
        std::string process(std::string const& s);
        void LoadFile(const std::string& modelFile) override;
        void LoadBasicTextures(const std::string& modelFile);
        void Init() override;
        void ReleaseFile() override;
        void ReadFile(std::string& in) override;
    };
}