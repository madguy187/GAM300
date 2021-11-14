#pragma once
#include "pch.h"
#include "GLM/glm/gtx/quaternion.hpp"

#include "Graphics/ModelFactory/AssimpModel/AssimpAnimator.h"

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

        Bone(mBone newBone);

        int GetPositionIndex(float animationTime);
        int GetScaleIndex(float animationTime);
        int GetRotationIndex(float animationTime);

        void Update(float animationTime);

        glm::mat4 InterpolatePosition(float animationTime);
        glm::mat4 InterpolateRotation(float animationTime);
        glm::mat4 InterpolateScaling(float animationTime);
        float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
    };

    struct BoneInfo
    {
        int id = 0;
        glm::mat4 offset{ 0.0f };
        std::string name;

        BoneInfo();

        BoneInfo(mBoneInfo newBoneInfo);

        BoneInfo(int _id, glm::mat4 _offset, std::array<char, 128> _name);
    };

    struct AssimpNodeData
    {
        glm::mat4 transformation{ 0 };
        std::array<char, 128> name{};
        int childrenCount = 0;
        std::vector<AssimpNodeData> children;
    };

    struct Animation
    {
        float modelLargestAxis = 1.0f;
        float m_Duration = 0.0f;
        int animationIndex = 0;
        int m_TicksPerSecond = 0;
        AnimationState m_AnimationState{ AnimationState::INVALID };
        std::string modelName;
        std::map<std::string, BoneInfo> m_BoneInfoMap;
        std::vector<Bone> m_Bones;
        AssimpNodeData m_RootNode;

        Animation();

        Animation(float axis, float duration, int ticks, AnimationState state, std::array<char, 128> name, std::vector<BoneInfo> boneInfo, std::vector<Bone> bones, AssimpNodeData rootNode);

        Bone* FindBone(std::string& name);
    };

    class AnimationManager
    {
        std::map<std::string, std::map<AnimationState, Animation>> animationMap;
    public:
        void RecurseChildren(AssimpNodeData& nodeData, std::fstream& AnimationFileRead);
        void CheckRecursionData(AssimpNodeData& nodeData);
        void InsertAnimation(Animation& newAnimation);

        std::map<std::string, std::map<AnimationState, Animation>>& GetAnimationMap();
        void CheckForAnimation(unsigned int ID);

        void CalculateBoneTransform(unsigned int ID, const AssimpNodeData* node, glm::mat4 parentTransform);
        void UpdateAnimation(unsigned int ID, float dt);

        void ChangeAnimationState(unsigned int ID, AnimationState state);
        AnimationState InitAnimationState(std::string modelName, std::string fileName);

        void SetAnimationSpeed(unsigned int ID, int speed);
    };
}