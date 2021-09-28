#include "pch.h"
#include "GeometryCompiler.h"
#include <string> 

namespace EclipseCompiler
{
    void GeometryCompiler::Init()
    {
        LoadFile("..//Eclipse//src//Assets//ASSModels");
    }

    void GeometryCompiler::LoadFile(const std::string& modelFile)
    {
        for (auto& dirEntry : std::filesystem::directory_iterator(modelFile))
        {
            const auto& path = dirEntry.path();
            auto relativePath = relative(path, "..//Eclipse//src//");
            std::string FolderName = relativePath.filename().string();

            std::string GoIntoModelFolder = ("..//Eclipse//src/Assets/ASSModels/" + FolderName);

            for (auto& dirEntry : std::filesystem::directory_iterator(GoIntoModelFolder))
            {
                const auto& FbxOrGltf = dirEntry.path();
                auto relativePath = relative(FbxOrGltf, "..//Eclipse//src//");
                std::string FbxOrGltfName = relativePath.filename().string();

                if (FbxOrGltfName.find("gltf") != std::string::npos || FbxOrGltfName.find("fbx") != std::string::npos)
                {
                    std::string PathName = ("..//Eclipse//src/Assets/ASSModels/" + FolderName + "/" + FbxOrGltfName).c_str();
                    std::unique_ptr<AssimpLoader> ptr = std::make_unique< AssimpLoader>();
                    ptr->LoadAssimpModel(PathName, Geometry);
                }
            }
        }
    }

    void GeometryCompiler::WriteToFile(std::unordered_map<std::string, Mesh>& In)
    {
        GeometryFile.open(Path + "GeometryFile/Geometry" + FileName,
            std::ios_base::out |
            std::ios_base::trunc |
            std::ios_base::binary);

        if (GeometryFile.fail())
        {
            std::cout << "Fail To Open Geometry File" << std::endl << std::endl;
            return;
        }

        int NumberOfModels = In.size();
        GeometryFile.write(reinterpret_cast<const char*>(&NumberOfModels), sizeof(NumberOfModels));

        for (auto i : In)
        {
            Mesh SaveModel = i.second;

            GeometryFile.write(reinterpret_cast<const char*>(&SaveModel), offsetof(Mesh, Vertices));

            int VertexSize = SaveModel.Vertices.size();
            GeometryFile.write(reinterpret_cast<const char*>(&VertexSize), sizeof(VertexSize));
            GeometryFile.write(reinterpret_cast<const char*>(SaveModel.Vertices.data()), sizeof(Vertex) * VertexSize);

            int IndicesSize = SaveModel.Indices.size();
            GeometryFile.write(reinterpret_cast<const char*>(&IndicesSize), sizeof(IndicesSize));
            GeometryFile.write(reinterpret_cast<const char*>(SaveModel.Indices.data()), sizeof(unsigned int) * IndicesSize);
        }

        GeometryFile.close();
    }

    void GeometryCompiler::ReleaseFile(std::string& in)
    {
        if (in == "1")
        {
            if (Geometry.empty())
            {
                std::cout << "No Geometry Loaded" << std::endl << std::endl;
                return;
            }

            WriteToFile(Geometry);
            std::cout << "Geometry File Produced" << std::endl << std::endl;
        }
    }

    void GeometryCompiler::ReadFile(std::string& in)
    {
        if (in == "Geo")
        {
            ReadFile();

            std::cout << "Read File " << std::endl << std::endl;
        }
    }

    void GeometryCompiler::ReadFile()
    {
        GeometryFileWrite.open(Path + "GeometryFile/Geometry" + FileName,
            std::ios::in |
            std::ios::binary);

        if (GeometryFileWrite.fail())
        {
            std::cout << "Fail To Open Geometry File" << std::endl << std::endl;
            return;
        }

        int VerticesSize = 0;
        int IndicesSize = 0;
        int TotalNumberOfModels = 0;

        // See how many Models
        GeometryFileWrite.read(reinterpret_cast<char*>(&TotalNumberOfModels), sizeof(int));

        for (int i = 0; i < TotalNumberOfModels; i++)
        {
            Mesh B;

            GeometryFileWrite.read(reinterpret_cast<char*>(&B), offsetof(Mesh, Vertices));

            GeometryFileWrite.read(reinterpret_cast<char*>(&VerticesSize), sizeof(VerticesSize));
            B.Vertices.resize(VerticesSize);
            GeometryFileWrite.read(reinterpret_cast<char*>(B.Vertices.data()), sizeof(Vertex) * VerticesSize);

            GeometryFileWrite.read(reinterpret_cast<char*>(&IndicesSize), sizeof(IndicesSize));
            B.Indices.resize(IndicesSize);
            GeometryFileWrite.read(reinterpret_cast<char*>(B.Indices.data()), sizeof(unsigned int) * IndicesSize);
        }
    }
}