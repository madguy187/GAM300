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

        GeometryFile.open("../Eclipse/src/Assets/Compilers/GeometryFile/Geometry.bin",
            std::ios_base::out |
            std::ios_base::trunc |
            std::ios_base::binary);

        for (auto& i : In)
        {

        }
#ifndef NEW_IMPLEMENTATION
        int totaloffset = 0;

        MeshA A;
        strcpy_s(A.MeshName.data(), A.MeshName.size(), "hell");
        A.MeshName[5] = '\0';

        A.Diffuse = glm::vec4{ 1.1, 1.2, 1.3, 11.4 };
        A.Specular = glm::vec4{ 5.5, 6.6 , 7.7 , 8.8 };
        A.Ambient = glm::vec4{ 3,4,5,6 };

        Vertex C;
        C.Position = { 1,2,3 };
        C.Normal = { 4,5,6 };
        C.TextureCoodinates = { 7,8 };
        C.m_Color = { 9,10,11,12 };

        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);
        A.Vertices.push_back(C);


        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(1);
        A.Indices.push_back(2);
        A.Indices.push_back(2);

        // Writing 
        // Diffuse , Specular , Ambient , Meshname , NoTex
        // glm::vec4 , glm::vec4 , glm::vec4 , 
        GeometryFile.write(reinterpret_cast<const char*>(&A), offsetof(MeshA, Vertices));

        int VertexSize = A.Vertices.size();
        GeometryFile.write(reinterpret_cast<const char*>(&VertexSize), sizeof(VertexSize));
        GeometryFile.write(reinterpret_cast<const char*>(A.Vertices.data()), sizeof(Vertex) * VertexSize);

        int IndicesSize = A.Indices.size();
        GeometryFile.write(reinterpret_cast<const char*>(&IndicesSize), sizeof(IndicesSize));
        GeometryFile.write(reinterpret_cast<const char*>(A.Indices.data()), sizeof(unsigned int) * IndicesSize);

        GeometryFile.close();

        ReadFile();
#else
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
#endif
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

    void GeometryCompiler::CalculateOffsets()
    {
    }

    void GeometryCompiler::ReadFile()
    {
        GeometryFileWrite.open("../Eclipse/src/Assets/Compilers/GeometryFile/Geometry.bin",
            std::ios::in |
            std::ios::binary);

        std::string eachline;

#ifndef NEW_IMPLEMENTATION

        MeshA B;

        int VerticesSize = 0;
        int IndicesSize = 0;

        GeometryFileWrite.read(reinterpret_cast<char*>(&B), offsetof(MeshA, Vertices));

        GeometryFileWrite.read(reinterpret_cast<char*>(&VerticesSize), sizeof(VerticesSize));
        B.Vertices.resize(VerticesSize);
        GeometryFileWrite.read(reinterpret_cast<char*>(B.Vertices.data()), sizeof(Vertex) * VerticesSize);

        GeometryFileWrite.read(reinterpret_cast<char*>(&IndicesSize), sizeof(IndicesSize));
        B.Indices.resize(IndicesSize);
        GeometryFileWrite.read(reinterpret_cast<char*>(B.Indices.data()), sizeof(unsigned int) * IndicesSize);

        int i = 0;
#else   
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
            //if (eachline == "-")
                //std::cout << "Vertices Next" << std::endl;

            while (std::getline(GeometryFileWrite, eachline))
            {
                if (eachline == "--")
                {
                    //std::cout << "Indices Next" << std::endl;
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
                    //std::cout << "Next Model" << std::endl;
                    break;
                }

                Model.Indices.push_back(std::atoi(eachline.c_str()));
            }

            Geometry.emplace(Model.MeshName, std::make_unique<Mesh>(Model));
    }
#endif
}
}