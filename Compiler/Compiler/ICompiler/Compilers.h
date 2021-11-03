#pragma once
#include <thread>

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
        std::string Path = "../Compiler/CompilerKeyFiles/";
        std::string FileName = ".eclipse";

        virtual void LoadFile(const std::string& modelFile) = 0;
        virtual void Init() = 0;
        virtual void ReleaseFile() = 0;
        virtual void ReadFile(std::string& in) = 0;
    };

    class CompilerManager
    {
    private:
        static inline std::vector<ICompiler*> AllCompilerContainer;
        static inline std::unordered_map<std::string, std::unique_ptr<std::thread>> CompilerThreads;

    public:
        void Register(ICompiler& in);
        void Initialise();
        void ProduceFile();
        void ReadFile(std::string& in);

        static void InitGeometry();
        static void InitTextures();
        static void InitPrefabs();
        static void InitAnimation();
        static void ReleaseGeometry();
        static void ReleaseTextures();
        static void ReleasePrefabs();
        static void ReleaseAnimation();
    };
}