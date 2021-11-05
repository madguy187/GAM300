#pragma once
#include "pch.h"
#include "GLM/glm/gtx/quaternion.hpp"

#define MAX_CHILDREN_NODE 100

namespace Eclipse
{
    struct KeyRotation
    {
        glm::quat orientation;
        float timeStamp;
    };

    struct KeyScale
    {
        glm::vec3 scale;
        float timeStamp;
    };

    struct KeyPosition
    {
        glm::vec3 position;
        float timeStamp;
    };

    struct Bone
    {
        std::vector<KeyPosition> m_Positions;
        std::vector<KeyRotation> m_Rotations;
        std::vector<KeyScale> m_Scales;

        int m_NumPositions;
        int m_NumRotations;
        int m_NumScalings;

        int m_ID;
        glm::mat4 m_LocalTransform;
        std::array<char, 128> BoneName;
    };

    struct BoneInfo
    {
        int id;
        glm::mat4 offset;
        std::array<char, 128> name;
    };

    struct AssimpNodeData
    {
        glm::mat4 transformation;
        std::array<char, 128> name;
        int childrenCount = 0;
        std::vector<AssimpNodeData> children;
    };

    struct AnimationData
    {
        float m_Duration;
        int m_TicksPerSecond;
        std::array<char, 128> modelName;
        std::vector<BoneInfo> m_BoneInfo;
        std::vector<Bone> m_Bones;
        AssimpNodeData m_RootNode;
        //std::map<std::string, BoneInfo> m_BoneInfoMap;
    };

    class AnimationManager
    {
        std::map<std::string, AnimationData> animationMap;
    public:
        void RecurseChildren(AssimpNodeData& nodeData, std::fstream& AnimationFileRead);
        void CheckRecursionData(AssimpNodeData& nodeData);
    };
}