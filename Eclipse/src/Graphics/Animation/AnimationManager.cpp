#include "pch.h"
#include "AnimationManager.h"
#include "Global.h"

void Eclipse::AnimationManager::RecurseChildren(AssimpNodeData& nodeData, std::fstream& AnimationFileRead)
{
    glm::mat4 transform;
    glm::mat4 parentTrans;
    std::array<char, 128> name;
    std::array<char, 128> parentName;
    int childCount;
    int parentCount;

    for (unsigned int i = 0; i < static_cast<unsigned int>(nodeData.childrenCount); ++i)
    {
        AnimationFileRead.read(reinterpret_cast<char*>(&parentName), sizeof(parentName));
        AnimationFileRead.read(reinterpret_cast<char*>(&parentCount), sizeof(int));
        AnimationFileRead.read(reinterpret_cast<char*>(&parentTrans), sizeof(glm::mat4));

        AnimationFileRead.read(reinterpret_cast<char*>(&transform), sizeof(glm::mat4));
        AnimationFileRead.read(reinterpret_cast<char*>(&name), sizeof(name));
        AnimationFileRead.read(reinterpret_cast<char*>(&childCount), sizeof(int));

        nodeData.name = parentName;
        nodeData.childrenCount = parentCount;
        nodeData.transformation = parentTrans;

        if (childCount != 0)
        {
            nodeData.name = parentName;
            nodeData.childrenCount = parentCount;
            nodeData.children[i].children.resize(childCount);
            nodeData.children[i].transformation = transform;
            nodeData.children[i].name = name;
            nodeData.children[i].childrenCount = childCount;

            AnimationFileRead.read(reinterpret_cast<char*>(nodeData.children[i].children.data()), sizeof(nodeData.children[i].children));

            RecurseChildren(nodeData.children[i], AnimationFileRead);
        }
        else
        {
            nodeData.children[i].name = name;
            nodeData.children[i].transformation = transform;
            nodeData.children[i].childrenCount = childCount;
        }
    }
}

void Eclipse::AnimationManager::CheckRecursionData(AssimpNodeData& nodeData)
{
    for (unsigned int i = 0; i < static_cast<unsigned int>(nodeData.childrenCount); ++i)
    {
        std::cout << "i: " << i << std::endl;
        std::cout << "Parent Name: " << nodeData.name.data() << std::endl;
        std::cout << "ChildrenCount: " << nodeData.children[i].childrenCount << std::endl;

        if (nodeData.children[i].childrenCount != 0)
        {
            std::cout << "Children Name: " << nodeData.children[i].name.data() << std::endl;
            std::cout << "Transformation[0][0]: " << nodeData.children[i].transformation[0][0] << std::endl;
            std::cout << "Transformation[0][1]: " << nodeData.children[i].transformation[0][1] << std::endl;
            std::cout << "Transformation[0][2]: " << nodeData.children[i].transformation[0][2] << std::endl;
            std::cout << "Transformation[0][3]: " << nodeData.children[i].transformation[0][3] << std::endl;
            std::cout << "Transformation[1][0]: " << nodeData.children[i].transformation[1][0] << std::endl;
            std::cout << "Transformation[1][1]: " << nodeData.children[i].transformation[1][1] << std::endl;
            std::cout << "Transformation[1][2]: " << nodeData.children[i].transformation[1][2] << std::endl;
            std::cout << "Transformation[1][3]: " << nodeData.children[i].transformation[1][3] << std::endl;
            std::cout << "Transformation[2][0]: " << nodeData.children[i].transformation[2][0] << std::endl;
            std::cout << "Transformation[2][1]: " << nodeData.children[i].transformation[2][1] << std::endl;
            std::cout << "Transformation[2][2]: " << nodeData.children[i].transformation[2][2] << std::endl;
            std::cout << "Transformation[2][3]: " << nodeData.children[i].transformation[2][3] << std::endl;
            std::cout << "Transformation[3][0]: " << nodeData.children[i].transformation[3][0] << std::endl;
            std::cout << "Transformation[3][1]: " << nodeData.children[i].transformation[3][1] << std::endl;
            std::cout << "Transformation[3][2]: " << nodeData.children[i].transformation[3][2] << std::endl;
            std::cout << "Transformation[3][3]: " << nodeData.children[i].transformation[3][3] << std::endl;
            std::cout << std::endl;

            CheckRecursionData(nodeData.children[i]);
        }
    }
}

void Eclipse::AnimationManager::InsertAnimation(Animation& newAnimation)
{
    animationMap[newAnimation.modelName][newAnimation.m_AnimationState] = newAnimation;
}

std::map<std::string, std::map<AnimationState, Animation>>& Eclipse::AnimationManager::GetAnimationMap()
{
    return animationMap;
}

void Eclipse::AnimationManager::CheckForAnimation(unsigned int ID)
{
    if (engine->world.CheckComponent<MeshComponent>(ID))
    {
        auto& mesh = engine->world.GetComponent<MeshComponent>(ID);
        std::string meshName = std::string(mesh.MeshName.data());

        for (auto& it : animationMap)
        {
            if (it.first.compare(meshName) == 0)
            {
                if (!engine->world.CheckComponent<AnimationComponent>(ID))
                {
                    AnimationComponent animCom{};
                    engine->world.AddComponent(ID, animCom);
                }

                auto& animation = engine->world.GetComponent<AnimationComponent>(ID);

                animation.m_CurrentAnimation = it.second.begin()->second;
                animation.modelLargestAxis = it.second.begin()->second.modelLargestAxis;
                animation.m_CurrentTime = 0.0f;
                //animation.m_Transforms.reserve(100);
                //
                //for (unsigned int i = 0; i < 100; ++i)
                //{
                //    animation.m_Transforms.push_back(glm::mat4(1.0f));
                //}

                if (!strcmp(meshName.c_str(), "Frog"))
                {
                    engine->gFSM.AddFSM(ID);
                    animation.m_CurrentAnimation.m_AnimationState = AnimationState::IDLE;
                    engine->gFSM.FindFSM(ID)->SetState(animation.m_CurrentAnimation.m_AnimationState);
                    ChangeAnimationState(ID, animation.m_CurrentAnimation.m_AnimationState);
                }
                else if (!strcmp(meshName.c_str(), "Body"))
                {
                    engine->gFSM.AddFSM(ID);
                    animation.m_CurrentAnimation.m_AnimationState = AnimationState::WALK;
                    engine->gFSM.FindFSM(ID)->SetState(animation.m_CurrentAnimation.m_AnimationState);
                    ChangeAnimationState(ID, animation.m_CurrentAnimation.m_AnimationState);
                }
                else if (!strcmp(meshName.c_str(), "MutantMesh"))
                {
                    engine->gFSM.AddFSM(ID);
                    animation.m_CurrentAnimation.m_AnimationState = AnimationState::MOTION;
                    engine->gFSM.FindFSM(ID)->SetState(animation.m_CurrentAnimation.m_AnimationState);
                    ChangeAnimationState(ID, animation.m_CurrentAnimation.m_AnimationState);
                }
            }
        }
    }
}

void Eclipse::AnimationManager::CalculateBoneTransform(unsigned int ID, const AssimpNodeData* node, glm::mat4 parentTransform)
{
    std::string nodeName = std::string(node->name.data());
    glm::mat4 nodeTransform = node->transformation;

    auto& animation = engine->world.GetComponent<AnimationComponent>(ID);

    Bone* Bone = animation.m_CurrentAnimation.FindBone(nodeName);

    if (Bone)
    {
        Bone->Update(animation.m_CurrentTime);
        nodeTransform = Bone->m_LocalTransform;
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    auto boneInfoMap = animation.m_CurrentAnimation.m_BoneInfoMap;
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        animation.m_Transforms[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
    {
        CalculateBoneTransform(ID, &node->children[i], globalTransformation);
    }
}

void Eclipse::AnimationManager::UpdateAnimation(unsigned int ID, float dt)
{
    auto& animation = engine->world.GetComponent<AnimationComponent>(ID);

    animation.m_DeltaTime = dt;

    animation.m_CurrentTime += animation.m_CurrentAnimation.m_TicksPerSecond * dt;
    animation.m_CurrentTime = fmod(animation.m_CurrentTime, animation.m_CurrentAnimation.m_Duration);
    CalculateBoneTransform(ID, &animation.m_CurrentAnimation.m_RootNode, glm::mat4(1.0f));
}

void Eclipse::AnimationManager::ChangeAnimationState(unsigned int ID, AnimationState state)
{
    if (engine->world.CheckComponent<AnimationComponent>(ID))
    {
        auto& animation = engine->world.GetComponent<AnimationComponent>(ID);
        std::string model = animation.m_CurrentAnimation.modelName;

        animation.m_CurrentAnimation = engine->gAnimationManager.GetAnimationMap()[model][state];
    }
}

AnimationState Eclipse::AnimationManager::InitAnimationState(std::string modelName, std::string fileName)
{
    if (modelName.compare("Frog") == 0)
    {
        if (fileName.compare("idle") == 0)
        {
            return AnimationState::IDLE;
        }
        else if (fileName.compare("run") == 0)
        {
            return AnimationState::RUN;
        }
        else if (fileName.compare("motion") == 0)
        {
            return AnimationState::MOTION;
        }
        else
        {
            EDITOR_LOG_WARN("WARNING! Some models have invalid animation state.");
            return AnimationState::INVALID;
        }
    }
    else if (modelName.compare("MutantMesh") == 0)
    {
        return AnimationState::DANCE;
    }
    else if (modelName.compare("Body") == 0)
    {
        if (fileName.compare("Breathing_Idle") == 0)
        {
            return AnimationState::IDLE;
        }
        else if (fileName.compare("Walking") == 0)
        {
            return AnimationState::WALK;
        }
        else if (fileName.compare("Great_Sword_Slash") == 0)
        {
            return AnimationState::SLASH;
        }
        else if (fileName.compare("Jogging") == 0)
        {
            return AnimationState::RUN;
        }
        else
        {
            EDITOR_LOG_WARN("WARNING! Some models have invalid animation state.");
            return AnimationState::INVALID;
        }
    }
    else
    {
        EDITOR_LOG_WARN("WARNING! Some models have invalid animation state.");
        return AnimationState::INVALID;
    }
}

void Eclipse::AnimationManager::SetAnimationSpeed(unsigned int ID, int speed)
{
    if (engine->world.CheckComponent<AnimationComponent>(ID))
    {
        auto& animation = engine->world.GetComponent<AnimationComponent>(ID);
        animation.m_CurrentAnimation.m_TicksPerSecond = speed;
    }
}

Eclipse::Bone::Bone(std::vector<KeyPosition> positions, std::vector<KeyRotation> rotations, std::vector<KeyScale> scales,
    int numPos, int numRot, int numScale, int id, glm::mat4 localTrans, std::array<char, 128> name) :
    m_Positions(positions), m_Rotations(rotations), m_Scales(scales),
    m_NumPositions(numPos), m_NumRotations(numRot), m_NumScalings(numScale),
    m_ID(id)
{
    m_LocalTransform = localTrans;
    BoneName = std::string(name.data());
}

Eclipse::Bone::Bone(mBone newBone)
{
    std::vector<KeyPosition> keyPosVec;
    for (auto& it : newBone.m_Positions)
    {
        KeyPosition newKeyPos;
        newKeyPos.position = it.position;
        newKeyPos.timeStamp = it.timeStamp;
        keyPosVec.push_back(newKeyPos);
    }

    m_Positions = keyPosVec;

    std::vector<KeyRotation> keyRotVec;
    for (auto& it : newBone.m_Rotations)
    {
        KeyRotation newKeyRot;
        newKeyRot.orientation = it.orientation;
        newKeyRot.timeStamp = it.timeStamp;
        keyRotVec.push_back(newKeyRot);
    }

    m_Rotations = keyRotVec;

    std::vector<KeyScale> keyScaleVec;
    for (auto& it : newBone.m_Scales)
    {
        KeyScale newKeyScale;
        newKeyScale.scale = it.scale;
        newKeyScale.timeStamp = it.timeStamp;
        keyScaleVec.push_back(newKeyScale);
    }

    m_Scales = keyScaleVec;

    m_NumPositions = newBone.m_NumPositions;
    m_NumRotations = newBone.m_NumRotations;
    m_NumScalings = newBone.m_NumScalings;

    m_ID = newBone.m_ID;
    m_LocalTransform = newBone.m_LocalTransform;
    BoneName = std::string(newBone.BoneName.data());
}

int Eclipse::Bone::GetPositionIndex(float animationTime)
{
    for (int index = 0; index < m_NumPositions - 1; ++index)
    {
        if (animationTime < m_Positions[static_cast<size_t>(index) + 1].timeStamp)
        {
            return index;
        }
    }

    assert(0);

    return 0;
}

int Eclipse::Bone::GetScaleIndex(float animationTime)
{
    for (int index = 0; index < m_NumScalings - 1; ++index)
    {
        if (animationTime < m_Scales[static_cast<size_t>(index) + 1].timeStamp)
        {
            return index;
        }
    }

    assert(0);

    return 0;
}

int Eclipse::Bone::GetRotationIndex(float animationTime)
{
    for (int index = 0; index < m_NumRotations - 1; ++index)
    {
        if (animationTime < m_Rotations[static_cast<size_t>(index) + 1].timeStamp)
        {
            return index;
        }
    }

    assert(0);

    return 0;
}

void Eclipse::Bone::Update(float animationTime)
{
    glm::mat4 translation = InterpolatePosition(animationTime);
    glm::mat4 rotation = InterpolateRotation(animationTime);
    glm::mat4 scale = InterpolateScaling(animationTime);

    m_LocalTransform = translation * rotation * scale;
}

glm::mat4 Eclipse::Bone::InterpolatePosition(float animationTime)
{
    if (m_NumPositions == 1)
    {
        return glm::translate(glm::mat4(1.0f), m_Positions[0].position);
    }

    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp, m_Positions[p1Index].timeStamp, animationTime);
    glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].position, m_Positions[p1Index].position, scaleFactor);

    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Eclipse::Bone::InterpolateRotation(float animationTime)
{
    if (m_NumRotations == 1)
    {
        auto rotation = glm::normalize(m_Rotations[0].orientation);
        return glm::toMat4(rotation);
    }

    int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp, m_Rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation, scaleFactor);

    finalRotation = glm::normalize(finalRotation);

    return glm::toMat4(finalRotation);
}

glm::mat4 Eclipse::Bone::InterpolateScaling(float animationTime)
{
    if (m_NumScalings == 1)
    {
        return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);
    }

    int p0Index = GetScaleIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp, m_Scales[p1Index].timeStamp, animationTime);
    glm::vec3 finalScale = glm::mix(m_Scales[p0Index].scale, m_Scales[p1Index].scale, scaleFactor);

    return glm::scale(glm::mat4(1.0f), finalScale);
}

float Eclipse::Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;

    return scaleFactor;
}

Eclipse::BoneInfo::BoneInfo()
{
}

Eclipse::BoneInfo::BoneInfo(mBoneInfo newBoneInfo)
{
    id = newBoneInfo.id;
    offset = newBoneInfo.offset;
    name = std::string(newBoneInfo.name.data());
}

Eclipse::BoneInfo::BoneInfo(int _id, glm::mat4 _offset, std::array<char, 128> _name) :
    id(_id)
{
    offset = _offset;
    name = std::string(_name.data());
}

Eclipse::Animation::Animation()
{
}

Eclipse::Animation::Animation(float axis, float duration, int ticks, AnimationState state, std::array<char, 128> name, std::vector<BoneInfo> boneInfo, std::vector<Bone> bones, AssimpNodeData rootNode) :
    modelLargestAxis(axis), m_Duration(duration), m_TicksPerSecond(ticks), m_AnimationState(state)
{
    modelName = std::string(name.data());

    for (auto& it : boneInfo)
    {
        m_BoneInfoMap.emplace(it.name, it);
    }

    m_Bones = bones;
    m_RootNode = rootNode;
}

Bone* Eclipse::Animation::FindBone(std::string& name)
{
    auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
        [&](const Bone& Bone)
        {
            return Bone.BoneName == name;
        }
    );

    if (iter == m_Bones.end())
    {
        return nullptr;
    }
    else
    {
        return &(*iter);
    }
}

