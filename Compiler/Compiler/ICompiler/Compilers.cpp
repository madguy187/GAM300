#include "pch.h"
#include "Compilers.h"

namespace EclipseCompiler
{
    void Helper::ProcessVec4(std::string& s, glm::vec4& vec4)
    {
        std::vector<int> T;
        std::string Finding = " ";

        size_t pos = 0;
        std::string token;
        while ((pos = s.find(Finding)) != std::string::npos)
        {
            token = s.substr(0, pos);

            T.push_back(std::stof(token.c_str()));
            s.erase(0, pos + Finding.length());
        }

        s.erase(0, pos + Finding.length());
        T.push_back(std::stof(s.c_str()));
        vec4 = glm::vec4{ T[0] , T[1] , T[2] , T[3] };
    }

    void Helper::ProcessVec3(std::string& s, glm::vec3& vec3)
    {
        std::vector<int> T;
        std::string Finding = " ";

        size_t pos = 0;
        std::string token;
        while ((pos = s.find(Finding)) != std::string::npos)
        {
            token = s.substr(0, pos);

            T.push_back(std::stof(token.c_str()));
            s.erase(0, pos + Finding.length());
        }

        s.erase(0, pos + Finding.length());
        T.push_back(std::stof(s.c_str()));
        vec3 = glm::vec3{ T[0] , T[1] , T[2] };
    }

    void Helper::ProcessVec2(std::string& s, glm::vec2& vec2)
    {
        std::vector<int> T;
        std::string Finding = " ";

        size_t pos = 0;
        std::string token;
        while ((pos = s.find(Finding)) != std::string::npos)
        {
            token = s.substr(0, pos);

            T.push_back(std::stof(token.c_str()));
            s.erase(0, pos + Finding.length());
        }

        s.erase(0, pos + Finding.length());
        T.push_back(std::stof(s.c_str()));
        vec2 = glm::vec2{ T[0] , T[1] };
    }

    glm::vec3 Helper::ReturnVec4(std::string& s)
    {
        std::vector<float> T;
        glm::vec4 NewT;
        std::string Finding = " ";

        size_t pos = 0;
        std::string token;
        while ((pos = s.find(Finding)) != std::string::npos)
        {
            token = s.substr(0, pos);
            T.push_back(std::stof(token.c_str()));
            s.erase(0, pos + Finding.length());
        }

        s.erase(0, pos + Finding.length());
        T.push_back(std::stof(s.c_str()));
        NewT = glm::vec4{ T[0] , T[1] , T[2] , T[3] };

        return NewT;
    }

    glm::vec3 Helper::ReturnVec3(std::string& s)
    {
        std::vector<float> T;
        glm::vec3 NewT;
        std::string Finding = " ";

        size_t pos = 0;
        std::string token;
        while ((pos = s.find(Finding)) != std::string::npos)
        {
            token = s.substr(0, pos);
            T.push_back(std::stof(token.c_str()));
            s.erase(0, pos + Finding.length());
        }

        s.erase(0, pos + Finding.length());
        T.push_back(std::stof(s.c_str()));
        NewT = glm::vec3{ T[0] , T[1] , T[2] };

        return NewT;
    }

    glm::vec2 Helper::ReturnVec2(std::string& s)
    {
        std::vector<int> T;
        glm::vec2 NewT;
        std::string Finding = " ";

        size_t pos = 0;
        std::string token;
        while ((pos = s.find(Finding)) != std::string::npos)
        {
            token = s.substr(0, pos);
            T.push_back(std::stof(token.c_str()));
            s.erase(0, pos + Finding.length());
        }

        s.erase(0, pos + Finding.length());
        T.push_back(std::stof(s.c_str()));
        NewT = glm::vec2{ T[0] , T[1] };

        return NewT;
    }

    void CompilerManager::Register(ICompiler& in)
    {
        AllCompilerContainer.push_back(&in);
    }

    void CompilerManager::Initialise()
    {
        for (auto& i : AllCompilerContainer)
        {
            i->Init();
        }
    }

    void CompilerManager::ProduceFile(std::string& in)
    {
        for (auto& i : AllCompilerContainer)
        {
            i->ReleaseFile(in);
        }
    }
}
