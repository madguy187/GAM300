#pragma once

namespace Eclipse
{
    struct Key
    {
        std::array<char, 128> MapName;
        InputKeycode Key;
        bool Stopper = false;
    };

    struct MouseKey
    {
        std::array<char, 128> MapName;
        InputMouseKeycode MouseKey;
        bool Stopper = false;
    };

    class InputCompiler
    {
    private:
        std::ofstream InputFileWrite;
        std::fstream InputFileRead;
        std::ofstream MouseFileWrite;
        std::fstream MouseFileRead;
        bool OpenFile(std::fstream& FileIn, std::string Path);
        void CloseFile(std::fstream& FileIn, std::string FileName = "");
        bool OpenFile(std::ofstream& FileIn, std::string Path);
        void CloseFile(std::ofstream& FileIn, std::string FileName = "");

        std::vector<std::unique_ptr<std::thread>> InputCompilerThreads;

        ///////////////////////////////////////////////////////////////////////////////////////////
        // For LOGICAL INPUT ( KEYBOARD )
        ///////////////////////////////////////////////////////////////////////////////////////////
        void SerializeLogicalInputs();
        void DeSerializeLogicalInputs();

        ///////////////////////////////////////////////////////////////////////////////////////////
        // For LOGICAL INPUT ( MOUSE )
        ///////////////////////////////////////////////////////////////////////////////////////////
        void SerializeMouseLogicalInputs();
        void DeSerializeMouseLogicalInputs();

    public:
        void Load();
        void Write();

        // Take Note that editor kep is the keycode.
        // Darren to check if need multimap

        void ProvideContainer(std::map<std::string, std::string>& In);
        void ReceiveMapping(std::map<std::string, std::string>& In);
    };
}