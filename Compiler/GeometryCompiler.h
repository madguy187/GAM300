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
        AssimpLoader Loader;
        std::ofstream GeometryFile;
        std::fstream GeometryFileWrite;
        int Test = 10;

    private:
        void LoadModels(const std::string& modelFile);
        void WriteToFile(std::unordered_map<std::string, std::unique_ptr<Mesh>>&);
        void ReadFile();

    public:
        void Init();
        void ReleaseFile(std::string& in);
    };
}