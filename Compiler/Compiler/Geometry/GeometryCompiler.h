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
        // Container storing each offsets for each geometry
        std::vector<std::vector<int>> OffsetContainer;
        std::ofstream GeometryFile;
        std::fstream GeometryFileWrite;
    private:
        void WriteToFile(std::unordered_map<std::string, Mesh>&);
        void ReadFile();

    public:
        void LoadFile(const std::string& modelFile) override;
        void Init() override;
        void ReleaseFile(std::string& in) override;
        void ReadFile(std::string& in) override;
    };
}