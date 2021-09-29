#pragma once
#include "Compilers.h"
#include "AssimpLoader.h"
#include <string>

namespace EclipseCompiler
{
    class PrefabsCompiler : public Helper, public ICompiler
    {
    public:
        // Prefab Container Names
        std::unordered_map<std::string, std::vector<std::string>> Prefabs;
        std::ofstream PrefabsFileWrite;
        std::fstream PrefabsFileRead;

    private:
        void WriteToFile();
        void ReadFile();

    public:
        void LoadFile(const std::string& modelFile) override;
        void Init() override;
        void ReleaseFile(std::string& in) override;
        void ReadFile(std::string& in) override;
        void GetPrefabs(std::unordered_map<std::string, std::vector<std::string>>& in);
    };
}