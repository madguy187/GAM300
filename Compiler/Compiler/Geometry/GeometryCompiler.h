#pragma once
#include "Compilers.h"
#include "AssimpLoader.h"
#include <string>

namespace EclipseCompiler
{
    class GeometryCompiler : public Helper, public ICompiler
    {
    public:
        std::unordered_map<std::string, std::unique_ptr<Mesh>> Geometry;
        std::ofstream GeometryFile;
        std::fstream GeometryFileWrite;

    private:
        void WriteToFile(std::unordered_map<std::string, std::unique_ptr<Mesh>>&);
        void ReadFile();

    public:
        void LoadFile(const std::string& modelFile) override;
        void Init() override;
        void ReleaseFile(std::string& in) override;
        void Write() override;
    };
}