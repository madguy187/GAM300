#include "pch.h"
#include "GeometryCompiler.h"

using namespace std;

namespace EclipseCompiler
{
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

    void GeometryCompiler::WriteToFile(std::unordered_map<std::string, std::unique_ptr<Mesh>>& In)
    {
        GeometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            GeometryFile.open("../Eclipse/src/Assets/Compilers/GeometryFile/Geometry.txt", std::fstream::in | std::fstream::out | std::fstream::binary | std::ios::trunc);
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "Exception opening/reading file" << e.what();
        }

        for (auto& i : In)
        {
            // Name , NoTextures , Diffuse , Specular , Ambient
            GeometryFile << i.first << std::endl;
            GeometryFile << (i.second->NoTex ? 1 : 0) << std::endl;
            GeometryFile << i.second->Diffuse.x << " " << i.second->Diffuse.y << " " << i.second->Diffuse.z << " " << i.second->Diffuse.w << std::endl;
            GeometryFile << i.second->Specular.x << " " << i.second->Specular.y << " " << i.second->Specular.z << " " << i.second->Specular.w << std::endl;
            GeometryFile << i.second->Ambient.x << " " << i.second->Ambient.y << " " << i.second->Ambient.z << " " << i.second->Ambient.z << std::endl;
            GeometryFile << "-" << std::endl;

            // Vertices
            // Position , Normal , TextureCoordinates
            for (auto& Vertices : i.second->Vertices)
            {
                GeometryFile << Vertices.Position.x << " " << Vertices.Position.y << " " << Vertices.Position.z << std::endl;
                GeometryFile << Vertices.Normal.x << " " << Vertices.Normal.y << " " << Vertices.Normal.z << std::endl;
                GeometryFile << Vertices.TextureCoodinates.x << " " << Vertices.TextureCoodinates.y << std::endl;
            }
            GeometryFile << "--" << std::endl;

            for (auto& Indices : i.second->Indices)
            {
                GeometryFile << Indices << std::endl;
            }
            GeometryFile << "---" << std::endl;

        }
        GeometryFile.close();
    }

    void GeometryCompiler::Init()
    {
        LoadFile("..//Eclipse//src//Assets//ASSModels");
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

    void GeometryCompiler::Write()
    {
    }

    void GeometryCompiler::ReadFile()
    {
        GeometryFileWrite.open("../Eclipse/src/Assets/Compilers/GeometryFile/Geometry.txt", std::ios::out | std::ios::in);
        std::string eachline;

        // This getline is to getName
        while (!GeometryFile.eof())
        {
            std::getline(GeometryFileWrite, eachline);

            if (eachline == "")
            {
                break;
            }

            Mesh Model;

            // Name
            Model.MeshName = eachline;

            // NoTextures
            std::getline(GeometryFileWrite, eachline);
            Model.NoTex = std::atoi(eachline.c_str());

            // Diffuse
            std::getline(GeometryFileWrite, eachline);
            ProcessVec4(eachline, Model.Diffuse);

            // Specular
            std::getline(GeometryFileWrite, eachline);
            ProcessVec4(eachline, Model.Specular);

            // Ambient
            std::getline(GeometryFileWrite, eachline);
            ProcessVec4(eachline, Model.Ambient);

            std::getline(GeometryFileWrite, eachline);
            if (eachline == "-")
                std::cout << "Vertices Next" << std::endl;

            while (std::getline(GeometryFileWrite, eachline))
            {
                if (eachline == "--")
                {
                    std::cout << "Indices Next" << std::endl;
                    break;
                }

                Vertex A;
                A.Position = ReturnVec3(eachline);

                std::getline(GeometryFileWrite, eachline);
                A.Normal = ReturnVec3(eachline);

                std::getline(GeometryFileWrite, eachline);
                A.TextureCoodinates = ReturnVec2(eachline);

                Model.Vertices.push_back(A);
            }

            while (std::getline(GeometryFileWrite, eachline))
            {
                if (eachline == "---")
                {
                    std::cout << "Next Model" << std::endl;
                    break;
                }

                Model.Indices.push_back(std::atoi(eachline.c_str()));
            }
            Geometry.emplace(Model.MeshName, std::make_unique<Mesh>(Model));
        }
    }
}