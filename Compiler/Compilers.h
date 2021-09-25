#pragma once
#include "AssimpLoader.h"
#include "CompilerSerializer.h"

namespace EclipseCompiler
{
    class Compilers
    {
    public:
        bool RegisterName = false;
        bool RegisterNoTex = false;
        bool RegisterDiffuse = false;
        bool RegisterSpecular = false;
        bool RegisterAmbient = false;

        std::unordered_map<std::string, std::unique_ptr<Mesh>> Geometry;
        AssimpLoader Loader;

        void LoadModels(const std::string& modelFile);
    };

    class CompilerManager
    {
    public:
        std::vector<std::unique_ptr<Compilers>> AllCompilers;
    };
}