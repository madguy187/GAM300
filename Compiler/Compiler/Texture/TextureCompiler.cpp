#include "pch.h"
#include "TextureCompiler.h"

namespace EclipseCompiler
{
    std::string TextureCompiler::process(std::string const& s)
    {
        std::string::size_type pos = s.find('.');
        if (pos != std::string::npos)
        {
            return s.substr(0, pos);
        }
        else
        {
            return s;
        }
    }

    void TextureCompiler::LoadFile(const std::string& modelFile)
    {
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

                if (FbxOrGltfName.find("gltf") != std::string::npos || FbxOrGltfName.find("fbx") != std::string::npos)
                {
                    std::string PathName = ("..//Eclipse//src/Assets/Models/" + FolderName + "/" + FbxOrGltfName).c_str();
                    std::unique_ptr<AssimpLoader> ptr = std::make_unique< AssimpLoader>();
                    ptr->LoadAssimpModelForTextures(PathName, NewTextureContainer);

                    int i = 0;
                }
            }
        }
    }

    void TextureCompiler::LoadBasicTextures(const std::string& modelFile)
    {
        for (auto& dirEntry : std::filesystem::directory_iterator(modelFile))
        {
            const auto& path = dirEntry.path();
            auto relativePath = relative(path, "..//Eclipse//src//");
            std::string TextureName = relativePath.filename().string();

            std::string NewPath = "src/Assets/Textures/";

            if (TextureName.find("png") != std::string::npos || TextureName.find("jpeg") != std::string::npos || TextureName.find("dds") != std::string::npos)
            {
                std::string NewCombined = NewPath + TextureName;
                std::string Name = process(TextureName);
                Texture tex(NewCombined);
                AllOtherTextureCotainer.emplace(Name, tex);
            }
        }
    }

    void TextureCompiler::Init()
    {
        LoadFile("..//Eclipse//src//Assets//Models");
        LoadBasicTextures("..//Eclipse//src//Assets//Textures");
    }

    void TextureCompiler::ReleaseFile()
    {
        if (NewTextureContainer.empty())
        {
            std::cout << "No Textures Loaded" << std::endl << std::endl;
            return;
        }

        WriteToFile(Textures);
        std::cout << "Textures File Produced" << std::endl << std::endl;
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
        int NumberOfTextures = NewTextureContainer.size();
        TextureFileWrite.write(reinterpret_cast<const char*>(&NumberOfTextures), sizeof(NumberOfTextures));

        for (auto const Textures : NewTextureContainer)
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

        /////////////////////

        TextureFileWrite.open(Path + "BasicTextureFile/Texture" + FileName,
            std::ios_base::out |
            std::ios_base::trunc |
            std::ios_base::binary);

        if (TextureFileWrite.fail())
        {
            std::cout << "Fail To Open Texture File" << std::endl << std::endl;
            return;
        }

        // Number Of Textures
        int NumberOfBasicTextures = AllOtherTextureCotainer.size();
        TextureFileWrite.write(reinterpret_cast<const char*>(&NumberOfBasicTextures), sizeof(NumberOfBasicTextures));

        for (auto const Textures : AllOtherTextureCotainer)
        {
            // Texture Name
            std::array<char, 128> TextureName;
            strcpy_s(TextureName.data(), TextureName.size(), Textures.first.data());
            TextureName[TextureName.size() - 1] = '\0';
            TextureFileWrite.write(reinterpret_cast<const char*>(&TextureName), sizeof(TextureName));

            // Texture Path
            std::array<char, 128> TexturePath;
            strcpy_s(TexturePath.data(), TexturePath.size(), Textures.second.TexturePath.data());
            TexturePath[TexturePath.size() - 1] = '\0';
            TextureFileWrite.write(reinterpret_cast<const char*>(&TexturePath), sizeof(TexturePath));
        }

        TextureFileWrite.close();
        ///////////////////////
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