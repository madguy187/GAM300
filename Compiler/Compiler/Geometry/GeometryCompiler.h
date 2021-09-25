#pragma once
#include "Compilers.h"
#include "AssimpLoader.h"
#include <string>

namespace EclipseCompiler
{
    class GeometryCompiler : public Compilers
    {
    public:
        std::unordered_map<std::string, std::unique_ptr<Mesh>> Geometry;
        std::ofstream GeometryFile;
        std::fstream GeometryFileWrite;

    private:
        void LoadFile(const std::string& modelFile);
        void WriteToFile(std::unordered_map<std::string, std::unique_ptr<Mesh>>&);
        void ReadFile();

    public:
        void Init();
        void ReleaseFile(std::string& in);
    };
}