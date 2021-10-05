#include "pch.h"
#include "EngineCompiler.h"

namespace Eclipse
{
    EngineCompiler::EngineCompiler()
    {

    }

    bool EngineCompiler::OpenFile(std::fstream& FileIn, std::string& Path)
    {
        FileIn.open(Path, std::ios::in | std::ios::binary);

        if (FileIn.fail())
        {
            EDITOR_LOG_WARN(("Fail to Open " + Path).c_str());
            return false;
        }

        return true;
    }

    void EngineCompiler::CloseFile(std::fstream& FileIn, std::string& FileName, unsigned int ContainerSize)
    {
        FileIn.close();
        EDITOR_LOG_INFO((FileName + "File Closed").c_str());

        if (ContainerSize != 0)
        {
            if (FileName == AllNames[0])
            {
                CompilerFlags.set(0, 1);
            }
            else if (FileName == AllNames[1])
            {
                CompilerFlags.set(1, 1);
            }
            else if (FileName == AllNames[2])
            {
                CompilerFlags.set(2, 1);
            }
            else if (FileName == AllNames[3])
            {
                CompilerFlags.set(3, 1);
            }
        }
    }

    void EngineCompiler::LoadGeometry()
    {
        if (OpenFile(GeometryFileRead, GeometryPath) == false)
            return;

        CompilerFlags.set(0, 0);

        EDITOR_LOG_INFO("Geometry File Open For Reading");

        unsigned int TotalNumberOfModels = 0;
        unsigned int VerticesSize = 0;
        unsigned int IndicesSize = 0;

        // See how many Models
        GeometryFileRead.read(reinterpret_cast<char*>(&TotalNumberOfModels), sizeof(TotalNumberOfModels));

        for (unsigned int i = 0; i < TotalNumberOfModels; i++)
        {
            VerticesSize = 0;
            IndicesSize = 0;

            Mesh B;

            auto hh = sizeof(B);

            GeometryFileRead.read(reinterpret_cast<char*>(&B), offsetof(Mesh, Vertices));

            GeometryFileRead.read(reinterpret_cast<char*>(&VerticesSize), sizeof(VerticesSize));
            B.Vertices.resize(VerticesSize);
            GeometryFileRead.read(reinterpret_cast<char*>(B.Vertices.data()), sizeof(Vertex) * VerticesSize);

            GeometryFileRead.read(reinterpret_cast<char*>(&IndicesSize), sizeof(IndicesSize));

            B.Indices.resize(IndicesSize);
            GeometryFileRead.read(reinterpret_cast<char*>(B.Indices.data()), sizeof(unsigned int) * IndicesSize);


            if (B.NoTex == false)
            {
                Mesh NewMesh(B.Vertices, B.Indices, B.MeshName.data(), B.Textures);
                std::string name = B.MeshName.data();
                engine->AssimpManager.InsertGeometry(name, NewMesh);
                engine->AssimpManager.InsertMeshName(name);
                engine->AssimpManager.InsertGeometryName(name);
            }
            else
            {
                Mesh NewMesh(B.Vertices, B.Indices, B.Diffuse, B.Specular, B.Ambient, B.NoTex, B.MeshName.data());
                std::string name = B.MeshName.data();
                engine->AssimpManager.InsertGeometry(name, NewMesh);
                engine->AssimpManager.InsertMeshName(name);
                engine->AssimpManager.InsertGeometryName(name);
            }
        }

        CloseFile(std::move(GeometryFileRead), AllNames[0], TotalNumberOfModels);
    }

    void EngineCompiler::LoadPrefabs()
    {
        if (OpenFile(PrefabsFileRead, PrefabsPath) == false)
            return;

        CompilerFlags.set(1, 0);

        EDITOR_LOG_INFO("Prefabs File Open For Reading");

        int TotalNumberOfPrefabs = 0;

        // See how many Prefabs
        PrefabsFileRead.read(reinterpret_cast<char*>(&TotalNumberOfPrefabs), sizeof(int));

        for (int i = 0; i < TotalNumberOfPrefabs; i++)
        {
            std::array<char, 128> ParentName;
            PrefabsFileRead.read(reinterpret_cast<char*>(&ParentName), sizeof(ParentName));
            ParentName[ParentName.size() - 1] = '\0';

            int NumberOfSubMeshes = 0;
            PrefabsFileRead.read(reinterpret_cast<char*>(&NumberOfSubMeshes), sizeof(NumberOfSubMeshes));

            for (int i = 0; i < NumberOfSubMeshes; i++)
            {
                std::array<char, 128> MeshName;
                PrefabsFileRead.read(reinterpret_cast<char*>(&MeshName), sizeof(MeshName));
                MeshName[MeshName.size() - 1] = '\0';

                engine->AssimpManager.InsertPrefabs(ParentName.data(), MeshName.data());
            }

            engine->AssimpManager.InsertMeshName(ParentName.data());
        }

        CloseFile(PrefabsFileRead, AllNames[1], TotalNumberOfPrefabs);
    }

    void EngineCompiler::LoadBasicTextures()
    {
        if (OpenFile(TextureFileRead, TexturePath) == false)
            return;

        CompilerFlags.set(2, 0);

        EDITOR_LOG_INFO("Texture File Open For Reading");

        // Number Of Textures
        int NumberOfBasicTextures = 0;
        TextureFileRead.read(reinterpret_cast<char*>(&NumberOfBasicTextures), sizeof(NumberOfBasicTextures));

        for (int i = 0; i < NumberOfBasicTextures; i++)
        {
            // Texture Name
            std::array<char, 128> TextureName;
            TextureFileRead.read(reinterpret_cast<char*>(&TextureName), sizeof(TextureName));

            // Texture DirecPathtory
            std::array<char, 128> TexturePath;
            TextureFileRead.read(reinterpret_cast<char*>(&TexturePath), sizeof(TexturePath));

            Texture tex(TexturePath.data());
            Graphics::textures.emplace(TextureName.data(), tex);
        }

        CloseFile(TextureFileRead, AllNames[2], NumberOfBasicTextures);
    }

    void EngineCompiler::LoadModelTextures()
    {
        if (OpenFile(ModelTextureFileRead, ModelTexturePath) == false)
            return;

        CompilerFlags.set(3, 0);

        EDITOR_LOG_INFO("Texture File Open For Reading");

        // Number Of Textures
        int NumberOfTextures = 0;
        ModelTextureFileRead.read(reinterpret_cast<char*>(&NumberOfTextures), sizeof(NumberOfTextures));

        for (int i = 0; i < NumberOfTextures; i++)
        {
            // Mesh Name
            std::array<char, 128> MeshName;
            ModelTextureFileRead.read(reinterpret_cast<char*>(&MeshName), sizeof(MeshName));

            // Number Of Textures
            int TextureType = 0;
            ModelTextureFileRead.read(reinterpret_cast<char*>(&TextureType), sizeof(TextureType));

            // Texture Directory
            std::array<char, 128> TextureDirectory;
            ModelTextureFileRead.read(reinterpret_cast<char*>(&TextureDirectory), sizeof(TextureDirectory));

            // Texture DirecPathtory
            std::array<char, 128> TexturePath;
            ModelTextureFileRead.read(reinterpret_cast<char*>(&TexturePath), sizeof(TexturePath));

            Texture tex(TextureDirectory.data(), TexturePath.data(), static_cast<aiTextureType>(TextureType));
            tex.Load(false);
            Graphics::textures.emplace(MeshName.data(), tex);
        }

        CloseFile(ModelTextureFileRead, AllNames[3], NumberOfTextures);
    }

    void EngineCompiler::RunCompiler()
    {
        system("Compiler.exe");
        EDITOR_LOG_INFO("FINISH EXECUTING COMPILER");
    }

    bool EngineCompiler::IsGeometryCompiled()
    {
        return CompilerFlags.test(0);
    }

    bool EngineCompiler::IsPrefabsCompiled()
    {
        return CompilerFlags.test(1);
    }
    bool EngineCompiler::IsModelTexturesCompiled()
    {
        return CompilerFlags.test(2);
    }

    bool EngineCompiler::IsBasicTexturesCompiled()
    {
        return CompilerFlags.test(3);
    }

    bool EngineCompiler::AreAllCompiled()
    {
        return IsGeometryCompiled() && IsPrefabsCompiled() && IsModelTexturesCompiled() && IsBasicTexturesCompiled();
    }

    void EngineCompiler::LoadCompilers()
    {
        std::thread CompilerWait{ &EngineCompiler::RunCompiler };
        CompilerWait.join();

        // Geometry Compiler
        LoadGeometry();
        LoadPrefabs();
        LoadModelTextures();
        LoadBasicTextures();

        if (AreAllCompiled() == true)
        {
            EDITOR_LOG_INFO("ALL COMPILERS LOADED , YOU CAN CREATE MODELS AND TEXTURES");
        }
        else
        {
            if (IsGeometryCompiled() == false)
            {
                EDITOR_LOG_WARN("GEOMETRY FILE NOT LOADED , YOU CANNOT CREATE MODELS");
            }

            if (IsPrefabsCompiled() == false)
            {
                EDITOR_LOG_WARN("PREFABS FILE NOT LOADED , YOU CANNOT CREATE MODELS");
            }

            if (IsModelTexturesCompiled() == false)
            {
                EDITOR_LOG_WARN("MODEL TEXTURES NOT LOADED , YOU CANNOT CREATE MODELS WITH TEXTURES");
            }

            if (IsBasicTexturesCompiled() == false)
            {
                EDITOR_LOG_WARN("BASIC TEXTURES NOT LOADED , YOU CANNOT CREATE USE BASIC TEXTURES");
            }
        }
    }

    void EngineCompiler::HotReload()
    {
        engine->AssimpManager.ClearAllMeshNames();
        engine->AssimpManager.ClearGeometry();
        engine->AssimpManager.ClearAllPrefabs();
        Graphics::textures.clear();

        std::thread CompilerWait{ &EngineCompiler::RunCompiler };
        CompilerWait.join();

        LoadGeometry();
        LoadPrefabs();
        LoadModelTextures();
        LoadBasicTextures();

        if (IsGeometryCompiled() == false)
        {
            EDITOR_LOG_WARN("GEOMETRY FILE NOT LOADED , YOU CANNOT CREATE MODELS");
        }

        if (IsPrefabsCompiled() == false)
        {
            EDITOR_LOG_WARN("PREFABS FILE NOT LOADED , YOU CANNOT CREATE MODELS");
        }

        if (IsModelTexturesCompiled() == false)
        {
            EDITOR_LOG_WARN("MODEL TEXTURES NOT LOADED");
        }

        if (IsBasicTexturesCompiled() == false)
        {
            EDITOR_LOG_WARN("BASIC TEXTURES NOT LOADED");
        }
    }

    void EngineCompiler::HotReloadTetxures()
    {
        Graphics::textures.clear();

        std::thread CompilerWait{ &EngineCompiler::RunCompiler };
        CompilerWait.join();

        LoadModelTextures();
        LoadBasicTextures();

        if (IsModelTexturesCompiled() == false)
        {
            EDITOR_LOG_WARN("MODEL TEXTURES NOT LOADED");
        }

        if (IsBasicTexturesCompiled() == false)
        {
            EDITOR_LOG_WARN("BASIC TEXTURES NOT LOADED");
        }
    }

    void EngineCompiler::Init()
    {
        LoadCompilers();
    }
}