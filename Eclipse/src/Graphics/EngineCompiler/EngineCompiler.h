#pragma once

namespace Eclipse
{
    class EngineCompiler
    {
    private:
        static inline std::vector<std::string> AllNames = { "GEOMETRY" , "PREFABS" , "BASICTEXTURES" , "TEXTURES" };
        // Touch Only When Needed
        static inline std::string GeometryPath = "../Compiler/CompilerKeyFiles/GeometryFile/Geometry.eclipse";
        static inline std::string PrefabsPath = "../Compiler/CompilerKeyFiles/PrefabsFile/Prefabs.eclipse";
        static inline std::string ModelTexturePath = "../Compiler/CompilerKeyFiles/TextureFile/Texture.eclipse";
        static inline std::string TexturePath = "../Compiler/CompilerKeyFiles/BasicTextureFile/Texture.eclipse";
        // Loading Geometry.eclipse File
        static inline std::fstream GeometryFileRead;
        // Loading Prefabs.eclipse File
        static inline std::fstream PrefabsFileRead;
        // Loading Textures.eclipse File
        static inline std::fstream TextureFileRead;
        // Loading Textures.eclipse File
        static inline std::fstream ModelTextureFileRead;
        static inline std::bitset<4> CompilerFlags;

        std::unordered_map<std::string, std::unique_ptr<std::thread>> CompilerThreads;

        // Load Geometries
        static void LoadGeometry();
        // Load Prefabs - This will Store all the Prefabs Name
        static void LoadPrefabs();
        // Load Basic Textures
        static void LoadBasicTextures();
        // Load Model Textures 
        static void LoadModelTextures();
        // Execute Compiler
        static void RunCompiler();
        void LoadCompilers();
        static bool OpenFile(std::fstream& FileIn, std::string& Path);
        static void CloseFile(std::fstream& FileIn, std::string& FileName, unsigned int ContainerSize);
    public:
        EngineCompiler();
        bool IsGeometryCompiled();
        bool IsPrefabsCompiled();
        bool IsModelTexturesCompiled();
        bool IsBasicTexturesCompiled();
        bool AreAllCompiled();
        void HotReload();
        void HotReloadTetxures();
        void Init();
    };
}