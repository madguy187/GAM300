#include "pch.h"
#include "Graphics/EngineCompiler/InputCompiler/InputCompiler.h"

namespace Eclipse
{
    bool InputCompiler::OpenFile(std::fstream& FileIn, std::string Path)
    {
        FileIn.open(Path, std::ios::in | std::ios::binary);

        if (FileIn.fail())
        {
            EDITOR_LOG_WARN(("Failed to open " + Path).c_str());
            return false;
        }

        return true;
    }

    void InputCompiler::CloseFile(std::fstream& FileIn, std::string FileName)
    {
        FileIn.close();
    }

    bool InputCompiler::OpenFile(std::ofstream& FileIn, std::string Path)
    {
        FileIn.open(Path, std::ios::out | std::ios::binary | std::ios_base::trunc);

        if (FileIn.fail())
        {
            EDITOR_LOG_WARN(("Failed to open " + Path).c_str());
            return false;
        }

        return true;
    }

    void InputCompiler::CloseFile(std::ofstream& FileIn, std::string FileName)
    {
        FileIn.close();
    }

    void InputCompiler::SerializeLogicalInputs()
    {
        if (!std::filesystem::exists("src//Assets//LogicalInput//Keyboard"))
        {
            std::filesystem::create_directories("src//Assets//LogicalInput//Keyboard");
        }

        OpenFile(InputFileWrite, "src/Assets/LogicalInput/Keyboard/LogicalInputMappings.input");

        int NumberOfLogicalInputs = engine->InputManager->KeyMappings.size();
        InputFileWrite.write(reinterpret_cast<const char*>(&NumberOfLogicalInputs), sizeof(NumberOfLogicalInputs));

        for (auto i : engine->InputManager->KeyMappings)
        {
            std::array<char, 128> StringName;
            strcpy_s(StringName.data(), StringName.size(), i.first.data());
            StringName[StringName.size() - 1] = '\0';

            Key A{ StringName , i.second };
            InputFileWrite.write(reinterpret_cast<const char*>(&A), offsetof(Key, Stopper));
        }
        CloseFile(InputFileWrite);
    }

    void InputCompiler::DeSerializeLogicalInputs()
    {
        engine->InputManager->KeyMappings.clear();
        OpenFile(InputFileRead, "src/Assets/LogicalInput/Keyboard/LogicalInputMappings.input");

        int TotalNumberOfInputs = 0;
        InputFileRead.read(reinterpret_cast<char*>(&TotalNumberOfInputs), sizeof(int));

        for (int i = 0; i < TotalNumberOfInputs; i++)
        {
            Key B;
            InputFileRead.read(reinterpret_cast<char*>(&B), offsetof(Key, Stopper));
            engine->InputManager->KeyMappings.emplace(B.MapName.data(), B.Key);
        }
        CloseFile(InputFileRead);
    }

    void InputCompiler::SerializeMouseLogicalInputs()
    {
        if (!std::filesystem::exists("src//Assets//LogicalInput//Mouse"))
        {
            std::filesystem::create_directories("src//Assets//LogicalInput//Mouse");
        }

        OpenFile(InputFileWrite, "src/Assets/LogicalInput/Mouse/LogicalMouseInputMappings.input");

        int MouseKeys = engine->InputManager->MouseMappings.size();
        InputFileWrite.write(reinterpret_cast<const char*>(&MouseKeys), sizeof(MouseKeys));

        for (auto i : engine->InputManager->MouseMappings)
        {
            std::array<char, 128> StringName;
            strcpy_s(StringName.data(), StringName.size(), i.first.data());
            StringName[StringName.size() - 1] = '\0';

            MouseKey A{ StringName ,i.second };
            InputFileWrite.write(reinterpret_cast<const char*>(&A), offsetof(MouseKey, Stopper));
        }
        CloseFile(InputFileWrite);
    }

    void InputCompiler::DeSerializeMouseLogicalInputs()
    {
        engine->InputManager->MouseMappings.clear();
        OpenFile(InputFileRead, "src/Assets/LogicalInput/Mouse/LogicalMouseInputMappings.input");

        int TotalNumberOfMouseKeys = 0;
        InputFileRead.read(reinterpret_cast<char*>(&TotalNumberOfMouseKeys), sizeof(int));

        for (int i = 0; i < TotalNumberOfMouseKeys; i++)
        {
            MouseKey B;
            InputFileRead.read(reinterpret_cast<char*>(&B), offsetof(MouseKey, Stopper));
            engine->InputManager->MouseMappings.emplace(B.MapName.data(), B.MouseKey);
        }
        CloseFile(InputFileRead);
    }
}

