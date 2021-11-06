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
        std::string BoneName;

        Bone(std::vector<KeyPosition> positions, std::vector<KeyRotation> rotations, std::vector<KeyScale> scales,
            int numPos, int numRot, int numScale, int id, glm::mat4 localTrans, std::array<char, 128> name);
        
    };

    struct BoneInfo
    {
        int id;
        glm::mat4 offset;
        std::string name;

        BoneInfo(int _id, glm::mat4 _offset, std::array<char, 128> _name);
    };

    struct AssimpNodeData
    {
        glm::mat4 transformation;
        std::array<char, 128> name;
        int childrenCount = 0;
        std::vector<AssimpNodeData> children;
    };

    struct Animation
    {
        float m_Duration = 0.0f;
        int m_TicksPerSecond = 0;
        std::string modelName;
        std::map<std::string, BoneInfo> m_BoneInfoMap;
        std::vector<Bone> m_Bones;
        AssimpNodeData m_RootNode;

        Animation();

        Animation(float duration, float ticks, std::array<char, 128> name, std::vector<BoneInfo> boneInfo, std::vector<Bone> bones, AssimpNodeData rootNode);
    };

    class AnimationManager
    {
        std::map<std::string, Animation> animationMap;
    public:
        void RecurseChildren(AssimpNodeData& nodeData, std::fstream& AnimationFileRead);
        void CheckRecursionData(AssimpNodeData& nodeData);
        void SetAnimationData(Animation& newAnimation, float duration, float ticks, std::array<char, 128> name, std::vector<BoneInfo> boneInfo, std::vector<Bone> bones, AssimpNodeData rootNode);
        void InsertAnimation(Animation& newAnimation);

        std::map<std::string, Animation>& GetAnimationMap();
        void CheckForAnimation(unsigned int ID);
    };
}