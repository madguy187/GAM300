#include "pch.h"
#include "GeometryCompiler.h"
#include <string> 

namespace EclipseCompiler
{
    void GeometryCompiler::Init()
    {
        LoadFile("..//Eclipse//src//Assets//Models");
    }

    void GeometryCompiler::LoadFile(const std::string& modelFile)
    {
        // We will do main Loading here
        for (auto& dirEntry : std::filesystem::directory_iterator(modelFile))
        {
            const auto& path = dirEntry.path();
            auto relativePath = relative(path, "..//Eclipse//src//");
            std::string FolderName = relativePath.filename().string();

            std::string GoIntoModelFolder = ("..//Eclipse//src/Assets/Models/" + FolderName);

            if (GoIntoModelFolder.find("HardReset.txt") != std::string::npos)
            {
                continue;
            }

            for (auto& dirEntry : std::filesystem::directory_iterator(GoIntoModelFolder))
            {
                const auto& FbxOrGltf = dirEntry.path();
                auto relativePath = relative(FbxOrGltf, "..//Eclipse//src//");
                std::string FbxOrGltfName = relativePath.filename().string();

                if (FbxOrGltfName.find("gltf") != std::string::npos || FbxOrGltfName.find("fbx") != std::string::npos || FbxOrGltfName.find("obj") != std::string::npos)
                {
                    std::string PathName = ("..//Eclipse//src/Assets/Models/" + FolderName + "/" + FbxOrGltfName).c_str();
                    std::unique_ptr<AssimpLoader> ptr = std::make_unique< AssimpLoader>();
                    ptr->LoadAssimpModel(PathName, Geometry);

                    // Get the Prefabs Mapping
                    for (auto& i : ptr->Meshes)
                    {
                        Prefabs[FolderName].push_back(i.MeshName.data());
                    }
                }
            }
        }
    }

    void GeometryCompiler::WriteToFile(std::unordered_map<std::string, Mesh>& In)
    {
        GeometryFileWrite.open(Path + "GeometryFile/Geometry" + FileName,
            std::ios_base::out |
            std::ios_base::trunc |
            std::ios_base::binary);

        if (GeometryFileWrite.fail())
        {
            std::cout << "Fail To Open Geometry Compiled File" << std::endl << std::endl;
            return;
        }

        //std::cout << "Writing to Geometry File " << std::endl;

        int NumberOfModels = In.size();
        //std::cout << "Detected Geometry Size " << NumberOfModels << std::endl << std::endl;
        GeometryFileWrite.write(reinterpret_cast<const char*>(&NumberOfModels), sizeof(NumberOfModels));

        for (auto i : In)
        {
            Mesh SaveModel = i.second;

            GeometryFileWrite.write(reinterpret_cast<const char*>(&SaveModel), offsetof(Mesh, Vertices));

            int VertexSize = SaveModel.Vertices.size();
            GeometryFileWrite.write(reinterpret_cast<const char*>(&VertexSize), sizeof(VertexSize));
            GeometryFileWrite.write(reinterpret_cast<const char*>(SaveModel.Vertices.data()), sizeof(Vertex) * VertexSize);

            int IndicesSize = SaveModel.Indices.size();
            GeometryFileWrite.write(reinterpret_cast<const char*>(&IndicesSize), sizeof(IndicesSize));
            GeometryFileWrite.write(reinterpret_cast<const char*>(SaveModel.Indices.data()), sizeof(unsigned int) * IndicesSize);
        }

        GeometryFileWrite.close();
        // std::cout << "Done Writing to Geometry File " << std::endl;
    }

    void GeometryCompiler::ReleaseFile()
    {
        WriteToFile(Geometry);
        //std::cout << "Geometry File Compiled" << std::endl << std::endl;
    }

    void GeometryCompiler::ReadFile(std::string& in)
    {
        if (in == "Geo")
        {
            if (Geometry.empty())
            {
                std::cout << "No Models Loaded" << std::endl << std::endl;
                return;
            }

            ReadFile();
            std::cout << "Read File " << std::endl << std::endl;
        }
    }

    void GeometryCompiler::ReadFile()
    {
        GeometryFileRead.open(Path + "GeometryFile/Geometry" + FileName,
            std::ios::in |
            std::ios::binary);

        if (GeometryFileRead.fail())
        {
            std::cout << "Fail To Open Geometry File" << std::endl << std::endl;
            return;
        }

        int TotalNumberOfModels = 0;

        // See how many Models
        GeometryFileRead.read(reinterpret_cast<char*>(&TotalNumberOfModels), sizeof(int));

        for (int i = 0; i < TotalNumberOfModels; i++)
        {
            int VerticesSize = 0;
            int IndicesSize = 0;

            Mesh B;

            GeometryFileRead.read(reinterpret_cast<char*>(&B), offsetof(Mesh, Vertices));

            GeometryFileRead.read(reinterpret_cast<char*>(&VerticesSize), sizeof(VerticesSize));
            B.Vertices.resize(VerticesSize);
            GeometryFileRead.read(reinterpret_cast<char*>(B.Vertices.data()), sizeof(Vertex) * VerticesSize);

            GeometryFileRead.read(reinterpret_cast<char*>(&IndicesSize), sizeof(IndicesSize));
            B.Indices.resize(IndicesSize);
            GeometryFileRead.read(reinterpret_cast<char*>(B.Indices.data()), sizeof(unsigned int) * IndicesSize);
        }

        GeometryFileRead.close();
    }
}