#include "pch.h"
#include "Graphics/EngineCompiler/MaterialCompiler/MaterialCompiler.h"

namespace Eclipse
{
    void MaterialCompiler::SerializeMaterials(MaterialInstance& materialref)
    {
        std::string NameOfMaterial = materialref.Name.data();
        OpenFile(MaterialFileWrite, "src/Assets/MaterialInstances/" + NameOfMaterial + ".mat");

        MaterialInstance A = materialref;
        MaterialFileWrite.write(reinterpret_cast<const char*>(&A), offsetof(MaterialInstance, Stopper));

        CloseFile(MaterialFileWrite, NameOfMaterial);
    }

    void MaterialCompiler::LoadMaterials()
    {
        engine->gPBRManager->ClearAllMaterialInstances();
        engine->gPBRManager->ClearAllMaterialInstNames();

        engine->gPBRManager->AllMaterialInstName.push_back("Default");

        for (auto& dirEntry : std::filesystem::directory_iterator("src//Assets//MaterialInstances"))
        {
            const auto& path = dirEntry.path();
            auto relativePath = relative(path, "src//");
            std::string FolderName = relativePath.filename().string();
            std::string GoIntoMaterialFolder = ("src/Assets/MaterialInstances/" + FolderName);

            //int Position = FolderName.find(".");
            //std::string GetMaterialName = FolderName.substr(0, Position);

            DeSerializeMaterials(FolderName);
        }
    }

    void MaterialCompiler::DeSerializeMaterials(std::string NameOfMaterial)
    {
        OpenFile(MaterialFileRead, "src/Assets/MaterialInstances/" + NameOfMaterial);
        MaterialInstance B;
        MaterialFileRead.read(reinterpret_cast<char*>(&B), offsetof(MaterialInstance, Stopper));
        engine->gPBRManager->AllMaterialInstances.emplace(B.Name.data(), std::make_unique<MaterialInstance>(B));
        engine->gPBRManager->AllMaterialInstName.push_back(B.Name.data());
        CloseFile(MaterialFileRead, NameOfMaterial);
    }

    bool MaterialCompiler::OpenFile(std::fstream& FileIn, std::string& Path)
    {
        FileIn.open(Path, std::ios::in | std::ios::binary);

        if (FileIn.fail())
        {
            EDITOR_LOG_WARN(("Failed to open " + Path).c_str());
            return false;
        }

        return true;
    }

    void MaterialCompiler::CloseFile(std::fstream& FileIn, std::string& FileName)
    {
        FileIn.close();
    }

    bool MaterialCompiler::OpenFile(std::ofstream& FileIn, std::string& Path)
    {
        FileIn.open(Path, std::ios::out | std::ios::binary | std::ios_base::trunc);

        if (FileIn.fail())
        {
            EDITOR_LOG_WARN(("Failed to open " + Path).c_str());
            return false;
        }

        return true;
    }

    void MaterialCompiler::CloseFile(std::ofstream& FileIn, std::string& FileName)
    {
        FileIn.close();
    }

}