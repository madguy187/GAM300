#include "pch.h"
#include "TextureCompiler.h"

namespace EclipseCompiler
{
    void TextureCompiler::LoadFile(const std::string& modelFile)
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
                    ptr->LoadAssimpModelForTextures(PathName, Textures);
                }
            }
        }

        //// Check How Many Textures are Loaded and which meshes are they mapped to?
        //std::cout << "Loaded Textures Count " << Textures.size() << std::endl;
        //std::cout << "-------------------------------------------------------------------" << std::endl;
        //for (auto const& Model : Textures)
        //{
        //    auto& ModelName = (Model.first);
        //    auto& MapWithMeshIndexAndTextures = (Model.second);

        //    std::cout << "Model Name : " << ModelName << std::endl;

        //    for (auto& i : MapWithMeshIndexAndTextures)
        //    {
        //        for (auto& EachTextures : i.second)
        //        {
        //            std::cout << "MeshIndex [" << i.first << "] " << EachTextures->TexturePath << std::endl;
        //        }
        //    }
        //    std::cout << std::endl;
        //}
    }

    void TextureCompiler::Init()
    {
        LoadFile("..//Eclipse//src//Assets//ASSModels");
        ReadFile();
    }

    void TextureCompiler::ReleaseFile(std::string& in)
    {
        if (in == "2")
        {
            if (Textures.empty())
            {
                std::cout << "No Textures Loaded" << std::endl << std::endl;
                return;
            }

            WriteToFile(Textures);
            std::cout << "Textures File Produced" << std::endl << std::endl;
        }
    }

    void TextureCompiler::Write()
    {
        int i = 0;
    }

    void TextureCompiler::CalculateOffsets()
    {
        //OffSetsForObject = { 4, sizeof(y), 4, sizeof(a) };
    }

    void TextureCompiler::WriteToFile(std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>& LoadedTextures)
    {
        TextureFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            TextureFile.open("../Eclipse/src/Assets/Compilers/TextureFile/Texture.txt", std::fstream::in | std::fstream::out | std::fstream::binary | std::ios::trunc);
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "Exception opening/reading file" << e.what();
        }

        for (auto const& Model : LoadedTextures)
        {
            // Save Name of Model
            auto& ModelName = (Model.first);
            TextureFile << ModelName << std::endl;

            auto& MapWithMeshIndexAndTextures = (Model.second);

            for (auto& i : MapWithMeshIndexAndTextures)
            {
                TextureFile << i.first << std::endl;
                //std::cout << "MeshIndex " << i.first << std::endl;

                TextureFile << i.second[0]->Type << std::endl;
                //std::cout << "Type " << i.second[0]->Type << std::endl;

                TextureFile << i.second[0]->TexturePath << std::endl;
                TextureFile << i.second[0]->TextureDirectory << std::endl;
                //std::cout << "Texture Path " << i.second[0]->TexturePath << std::endl;
                //std::cout << "Texture Dire " << i.second[0]->TextureDirectory << std::endl;
            }

            TextureFile << "---" << std::endl;
        }
    }

    void TextureCompiler::ReadFile()
    {
        TextureFileWrite.open("../Eclipse/src/Assets/Compilers/TextureFile/Texture.txt", std::ios::out | std::ios::in);
        std::string eachline;

        // This getline is to getName
        while (!TextureFileWrite.eof())
        {
            std::getline(TextureFileWrite, eachline);

            if (eachline == "")
            {
                break;
            }

            std::unique_ptr<Texture> TestTexture(new Texture());

            // Get Name of Mesh
            std::string Name = eachline;

            // Mesh Index
            std::getline(TextureFileWrite, eachline);
            int MeshIndex = std::atoi(eachline.c_str());

            // Type
            std::getline(TextureFileWrite, eachline);
            TestTexture->Type = static_cast<aiTextureType>(std::atoi(eachline.c_str()));

            // Path
            std::getline(TextureFileWrite, eachline);
            TestTexture->TexturePath = eachline;

            // Directory
            std::getline(TextureFileWrite, eachline);
            TestTexture->TextureDirectory = eachline;

            //Check isit End of A texture for mesh
            std::getline(TextureFileWrite, eachline);
            if (eachline == "---")
            {
                // Insert here
                continue;
            }
        }
    }
}