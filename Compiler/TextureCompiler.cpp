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

        // Check How Many Textures are Loaded and which meshes are they mapped to?
        std::cout << "Loaded Textures Count " << Textures.size() << std::endl;
        std::cout << "-------------------------------------------------------------------" << std::endl;
        for (auto const& Model : Textures)
        {
            auto& ModelName = (Model.first);
            auto& MapWithMeshIndexAndTextures = (Model.second);

            std::cout << "Model Name : " << ModelName << std::endl;

            for (auto& i : MapWithMeshIndexAndTextures)
            {
                for (auto& EachTextures : i.second)
                {
                    std::cout << "MeshIndex [" << i.first << "] " << EachTextures->TexturePath << std::endl;
                }
            }
            std::cout << std::endl;
        }
        std::cout << "-------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    }

    void TextureCompiler::Init()
    {
        LoadFile("..//Eclipse//src//Assets//ASSModels");
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

        for (auto& i : LoadedTextures)
        {
            TextureFile << "Hi" << std::endl;
        }
    }

    void TextureCompiler::InsertTextures(std::string& NameofModel, std::unique_ptr<Texture> in, unsigned int MeshId)
    {
        Textures[NameofModel][MeshId].push_back(std::move(in));
    }
}