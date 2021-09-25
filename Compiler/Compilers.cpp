#include "pch.h"
#include "Compilers.h"

namespace EclipseCompiler
{
    void Compilers::LoadModels(const std::string& modelFile)
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
                    ptr->LoadAssimpModel(PathName,Geometry);
                }
            }
        }
    }
}
