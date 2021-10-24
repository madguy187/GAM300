#include "pch.h"
#include "Graphics/EngineCompiler/MaterialCompiler/MaterialCompiler.h"

namespace Eclipse
{
    void MaterialCompiler::SerializeMaterials(MaterialInstance& materialref)
    {

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
        FileIn.open(Path, std::ios::in | std::ios::binary);

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