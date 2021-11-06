#include "pch.h"
#include "AnimationManager.h"

void Eclipse::AnimationManager::RecurseChildren(AssimpNodeData& nodeData, std::fstream& AnimationFileRead)
{
    glm::mat4 transform;
    std::array<char, 128> name;
    std::array<char, 128> parentName;
    int childCount;
    int parentCount;

    for (unsigned int i = 0; i < nodeData.childrenCount; ++i)
    {
        AnimationFileRead.read(reinterpret_cast<char*>(&parentName), sizeof(parentName));
        AnimationFileRead.read(reinterpret_cast<char*>(&parentCount), sizeof(int));
        AnimationFileRead.read(reinterpret_cast<char*>(&transform), sizeof(glm::mat4));
        AnimationFileRead.read(reinterpret_cast<char*>(&name), sizeof(name));
        AnimationFileRead.read(reinterpret_cast<char*>(&childCount), sizeof(int));

        //Garbage values
        if (childCount > MAX_CHILDREN_NODE || childCount < 0)
        {
            //nodeData.children[i].childrenCount = 0;
            AnimationFileRead.seekg(-(sizeof(glm::mat4) + sizeof(name) + sizeof(int)), AnimationFileRead.cur);
            return;
        }

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
    }
}

void Eclipse::AnimationManager::CheckRecursionData(AssimpNodeData& nodeData)
{
    for (unsigned int i = 0; i < nodeData.childrenCount; ++i)
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

void Eclipse::AnimationManager::SetAnimationData(Animation& newAnimation, float duration, float ticks, std::array<char, 128> name, std::vector<BoneInfo> boneInfo, std::vector<Bone> bones, AssimpNodeData rootNode)
{
    newAnimation.m_Duration = duration;
    newAnimation.m_TicksPerSecond = ticks;

    newAnimation.modelName = std::string(name.data());

    for (auto& it : boneInfo)
    {
        newAnimation.m_BoneInfoMap.emplace(it.name, it);
    }

    newAnimation.m_Bones = bones;
    newAnimation.m_RootNode = rootNode;
}

void Eclipse::AnimationManager::InsertAnimation(Animation& newAnimation)
{
    animationMap.emplace(newAnimation.modelName, newAnimation);
}

std::map<std::string, Animation>& Eclipse::AnimationManager::GetAnimationMap()
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
                    engine->world.AddComponent(ID, AnimationComponent{});
                }

                auto& animation = engine->world.GetComponent<AnimationComponent>(ID);

                animation.m_CurrentAnimation = it.second;
                animation.m_CurrentTime = 0.0f;
                animation.m_Transforms.reserve(100);

                for (unsigned int i = 0; i < 100; ++i)
                {
                    animation.m_Transforms.push_back(glm::mat4(1.0f));
                }
            }
        }
    }
}

Eclipse::Bone::Bone(std::vector<KeyPosition> positions, std::vector<KeyRotation> rotations, std::vector<KeyScale> scales, 
                    int numPos, int numRot, int numScale, int id, glm::mat4 localTrans, std::array<char, 128> name):
    m_Positions(positions), m_Rotations(rotations), m_Scales(scales), 
    m_NumPositions(numPos), m_NumRotations(numRot), m_NumScalings(numScale),
    m_ID(id)
{
    m_LocalTransform = localTrans;
    BoneName = std::string(name.data());
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

Eclipse::Animation::Animation(float duration, float ticks, std::array<char, 128> name, std::vector<BoneInfo> boneInfo, std::vector<Bone> bones, AssimpNodeData rootNode):
    m_Duration(duration), m_TicksPerSecond(ticks)
{
    modelName = std::string(name.data());

    for (auto& it : boneInfo)
    {
        m_BoneInfoMap.emplace(it.name, it);
    }

    m_Bones = bones;
    m_RootNode = rootNode;
}
