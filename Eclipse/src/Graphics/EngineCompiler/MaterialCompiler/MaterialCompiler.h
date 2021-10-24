#pragma once

namespace Eclipse
{
    class MaterialCompiler
    {
    public:
        std::ofstream MaterialFileWrite;
        std::fstream MaterialFileRead;

        void SerializeMaterials(MaterialInstance& materialref);

        bool OpenFile(std::fstream& FileIn, std::string& Path);
        void CloseFile(std::fstream& FileIn, std::string& FileName);

        bool OpenFile(std::ofstream& FileIn, std::string& Path);
        void CloseFile(std::ofstream& FileIn, std::string& FileName);
    };
}