#pragma once

namespace EclipseCompiler
{
    class Helper
    {
    public:
        void ProcessVec4(std::string& s, glm::vec4& vec4);
        void ProcessVec3(std::string& s, glm::vec3& vec3);
        void ProcessVec2(std::string& s, glm::vec2& vec2);

        glm::vec3 ReturnVec4(std::string& s);
        glm::vec3 ReturnVec3(std::string& s);
        glm::vec2 ReturnVec2(std::string& s);
    };

    class ICompiler
    {
    public:
        std::string Path = "../Eclipse/src/Assets/Compilers/";
        std::string FileName = ".eclipse";

        virtual void LoadFile(const std::string& modelFile) = 0;
        virtual void Init() = 0;
        virtual void ReleaseFile(std::string& in) = 0;
        virtual void ReadFile(std::string& in) = 0;
    };

    class CompilerManager
    {
    private:
        std::vector<ICompiler*> AllCompilerContainer;

    public:
        void Register(ICompiler& in);
        void Initialise();
        void ProduceFile(std::string& in);
        void ReadFile(std::string& in);
    };
}