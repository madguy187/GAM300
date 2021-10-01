#pragma once
#include "Compilers.h"
#include "AssimpLoader.h"
#include <string>

namespace EclipseCompiler
{
    class GeometryCompiler : public Helper, public ICompiler
    {
    public:
        // Geometry Container
        std::unordered_map<std::string,Mesh> Geometry;
        std::ofstream GeometryFileWrite;
        std::fstream GeometryFileRead;

        // Prefab Container Names
        std::unordered_map<std::string, std::vector<std::string>> Prefabs;

    private:
        void WriteToFile(std::unordered_map<std::string, Mesh>&);
        void ReadFile();

    public:
        void LoadFile(const std::string& modelFile) override;
        void Init() override;
        void ReleaseFile() override;
        void ReadFile(std::string& in) override;
    };
}