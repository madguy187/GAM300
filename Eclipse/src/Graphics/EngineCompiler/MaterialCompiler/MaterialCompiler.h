#pragma once

namespace Eclipse
{
    class MaterialCompiler
    {
    private:
        std::ofstream MaterialFileWrite;
        std::fstream MaterialFileRead;
        bool OpenFile(std::fstream& FileIn, std::string& Path);
        void CloseFile(std::fstream& FileIn, std::string& FileName);
        bool OpenFile(std::ofstream& FileIn, std::string& Path);
        void CloseFile(std::ofstream& FileIn, std::string& FileName);

    public:
        void SerializeMaterials(MaterialInstance& materialref);
        void LoadMaterials();
        void DeSerializeMaterials(std::string NameOfMaterial);
    };
}