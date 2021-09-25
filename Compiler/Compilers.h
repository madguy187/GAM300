#pragma once

namespace EclipseCompiler
{
    class Compilers
    {
    public:
        void ProcessVec4(std::string& s, glm::vec4& vec4);
        void ProcessVec3(std::string& s, glm::vec3& vec3);
        void ProcessVec2(std::string& s, glm::vec2& vec2);

        glm::vec3 ReturnVec4(std::string& s);
        glm::vec3 ReturnVec3(std::string& s);
        glm::vec2 ReturnVec2(std::string& s);
    };

    class CompilerManager
    {
    public:
        std::vector<std::unique_ptr<Compilers>> AllCompilers;
    };
}