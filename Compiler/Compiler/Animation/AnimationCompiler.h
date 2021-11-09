#pragma once
#include "Compilers.h"
#include "AssimpLoader.h"
#include <string>
#include <vector>
#include <fstream>

namespace EclipseCompiler
{
    class AnimationCompiler : public Helper, public ICompiler
    {
    public:
        // Animation Container
        static inline std::vector<AnimationData> Animation;
        std::ofstream AnimationFileWrite;
        std::fstream AnimationFileRead;

    private:
        void WriteToFile(std::vector<AnimationData>&);
        void ReadFile();

        void RecurseChildren(AssimpNodeData& nodeData);
    public:
        void LoadFile(const std::string& animationFile) override;
        void Init() override;
        void ReleaseFile() override;
        void ReadFile(std::string& in) override;
    };
}