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
