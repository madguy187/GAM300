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
                    ptr->LoadAssimpModelForTextures(PathName, TextureCotainer);
                }
            }
        }
    }

    void TextureCompiler::Init()
    {
        LoadFile("..//Eclipse//src//Assets//ASSModels");
    }

    void TextureCompiler::ReleaseFile(std::string& in)
    {
        if (in == "2")
        {
            if (TextureCotainer.empty())
            {
                std::cout << "No Textures Loaded" << std::endl << std::endl;
                return;
            }

            WriteToFile(Textures);
            std::cout << "Textures File Produced" << std::endl << std::endl;
        }
    }

    void TextureCompiler::ReadFile(std::string& in)
    {
        if (in == "Tex")
        {
            ReadFile();
        }
    }

    void TextureCompiler::WriteToFile(std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>& LoadedTextures)
    {
        TextureFileWrite.open(Path + "TextureFile/Texture" + FileName,
            std::ios_base::out |
            std::ios_base::trunc |
            std::ios_base::binary);

        if (TextureFileWrite.fail())
        {
            std::cout << "Fail To Open Geometry File" << std::endl << std::endl;
            return;
        }

        // Number Of Textures
        int NumberOfTextures = TextureCotainer.size();
        TextureFileWrite.write(reinterpret_cast<const char*>(&NumberOfTextures), sizeof(NumberOfTextures));

        for (auto const Textures : TextureCotainer)
        {
            // Mesh Name
            std::array<char, 128> MeshName;
            strcpy_s(MeshName.data(), MeshName.size(), Textures.first.data());
            MeshName[MeshName.size() - 1] = '\0';
            TextureFileWrite.write(reinterpret_cast<const char*>(&MeshName), sizeof(MeshName));

            // Number Of Textures
            int TextureType = Textures.second.Type;
            TextureFileWrite.write(reinterpret_cast<const char*>(&TextureType), sizeof(TextureType));

            // Texture Directory
            std::array<char, 128> TextureDirectory;
            strcpy_s(TextureDirectory.data(), TextureDirectory.size(), Textures.second.TextureDirectory.data());
            TextureDirectory[TextureDirectory.size() - 1] = '\0';
            TextureFileWrite.write(reinterpret_cast<const char*>(&TextureDirectory), sizeof(TextureDirectory));

            // Texture Path
            std::array<char, 128> TexturePath;
            strcpy_s(TexturePath.data(), TexturePath.size(), Textures.second.TexturePath.data());
            TexturePath[TexturePath.size() - 1] = '\0';
            TextureFileWrite.write(reinterpret_cast<const char*>(&TexturePath), sizeof(TexturePath));
        }

        TextureFileWrite.close();
    }

    void TextureCompiler::ReadFile()
    {
        std::unordered_map<std::string, Texture> TestRead;

        TextureFileRead.open(Path + "TextureFile/Texture" + FileName,
            std::ios::in |
            std::ios::binary);

        if (TextureFileRead.fail())
        {
            std::cout << "Fail To Open Texture File" << std::endl << std::endl;
            return;
        }

        // Number Of Textures
        int NumberOfTextures = 0;
        TextureFileRead.read(reinterpret_cast<char*>(&NumberOfTextures), sizeof(NumberOfTextures));

        for (int i = 0; i < NumberOfTextures; i++)
        {
            // Mesh Name
            std::array<char, 128> MeshName;
            TextureFileRead.read(reinterpret_cast<char*>(&MeshName), sizeof(MeshName));

            // Number Of Textures
            int TextureType = 0;
            TextureFileRead.read(reinterpret_cast<char*>(&TextureType), sizeof(TextureType));

            // Texture Directory
            std::array<char, 128> TextureDirectory;
            TextureFileRead.read(reinterpret_cast<char*>(&TextureDirectory), sizeof(TextureDirectory));

            // Texture DirecPathtory
            std::array<char, 128> TexturePath;
            TextureFileRead.read(reinterpret_cast<char*>(&TexturePath), sizeof(TexturePath));

            Texture tex(TextureDirectory.data(), TexturePath.data(), static_cast<aiTextureType>(TextureType));
            TestRead.emplace(MeshName.data(), tex);
        }

        TextureFileRead.close();
    }
}