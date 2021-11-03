#include "pch.h"
#include "AnimationCompiler.h"

void EclipseCompiler::AnimationCompiler::Init()
{
    LoadFile("..//Eclipse//src//Assets//Models");
}

void EclipseCompiler::AnimationCompiler::LoadFile(const std::string& animationFile)
{
    for (auto& dirEntry : std::filesystem::directory_iterator(animationFile))
    {
        const auto& path = dirEntry.path();
        auto relativePath = relative(path, "..//Eclipse//src//");
        std::string FolderName = relativePath.filename().string();

        std::string GoIntoModelFolder = ("..//Eclipse//src/Assets/Models/" + FolderName);

        if (GoIntoModelFolder.find("HardReset.txt") != std::string::npos)
        {
            continue;
        }

        for (auto& dirEntry : std::filesystem::directory_iterator(GoIntoModelFolder))
        {
            const auto& FbxOrGltf = dirEntry.path();
            auto relativePath = relative(FbxOrGltf, "..//Eclipse//src//");
            std::string FbxOrGltfName = relativePath.filename().string();

            if (FbxOrGltfName.find("gltf") != std::string::npos || FbxOrGltfName.find("fbx") != std::string::npos)
            {
                std::string PathName = ("..//Eclipse//src/Assets/Models/" + FolderName + "/" + FbxOrGltfName).c_str();
                std::unique_ptr<AssimpLoader> ptr = std::make_unique<AssimpLoader>();
                ptr->LoadAssimpAnimationModel(PathName);
                ptr->LoadAnimationData(PathName, Animation);
            }
        }
    }
}

void EclipseCompiler::AnimationCompiler::WriteToFile(std::vector<AnimationData>& In)
{
    AnimationFileWrite.open(Path + "AnimationFile/Animation" + FileName,
        std::ios_base::out |
        std::ios_base::trunc |
        std::ios_base::binary);

    if (AnimationFileWrite.fail())
    {
        std::cout << "Fail To Open Animation Compiled File" << std::endl << std::endl;
        return;
    }

    //std::cout << "Writing to Animation File " << std::endl;

    int animationNum = In.size();
    //std::cout << "Detected Animation Size " << animationNum << std::endl << std::endl;
    AnimationFileWrite.write(reinterpret_cast<const char*>(&animationNum), sizeof(animationNum));

    for (auto it : In)
    {
        //AnimationFileWrite.write(reinterpret_cast<const char*>(&it), offsetof(AnimationData, m_Bones));
        AnimationFileWrite.write(reinterpret_cast<const char*>(&it), offsetof(AnimationData, m_BoneInfo));

        int boneInfoSize = it.m_BoneInfo.size();
        AnimationFileWrite.write(reinterpret_cast<const char*>(&boneInfoSize), sizeof(boneInfoSize));
        AnimationFileWrite.write(reinterpret_cast<const char*>(it.m_BoneInfo.data()), sizeof(BoneInfo) * boneInfoSize);

        int boneSize = it.m_Bones.size();
        AnimationFileWrite.write(reinterpret_cast<const char*>(&boneSize), sizeof(boneSize));

        for (int i = 0; i < boneSize; ++i)
        {
            int keyPosSize = it.m_Bones[i].m_Positions.size();
            AnimationFileWrite.write(reinterpret_cast<const char*>(&keyPosSize), sizeof(keyPosSize));
            AnimationFileWrite.write(reinterpret_cast<const char*>(it.m_Bones[i].m_Positions.data()), sizeof(KeyPosition) * keyPosSize);

            int keyRotSize = it.m_Bones[i].m_Rotations.size();
            AnimationFileWrite.write(reinterpret_cast<const char*>(&keyRotSize), sizeof(keyRotSize));
            AnimationFileWrite.write(reinterpret_cast<const char*>(it.m_Bones[i].m_Rotations.data()), sizeof(KeyRotation) * keyRotSize);

            int keyScaleSize = it.m_Bones[i].m_Scales.size();
            AnimationFileWrite.write(reinterpret_cast<const char*>(&keyScaleSize), sizeof(keyScaleSize));
            AnimationFileWrite.write(reinterpret_cast<const char*>(it.m_Bones[i].m_Scales.data()), sizeof(KeyScale) * keyScaleSize);

            AnimationFileWrite.write(reinterpret_cast<const char*>(&it.m_Bones[i].m_NumPositions), sizeof(int));
            AnimationFileWrite.write(reinterpret_cast<const char*>(&it.m_Bones[i].m_NumRotations), sizeof(int));
            AnimationFileWrite.write(reinterpret_cast<const char*>(&it.m_Bones[i].m_NumScalings), sizeof(int));

            AnimationFileWrite.write(reinterpret_cast<const char*>(&it.m_Bones[i].m_ID), sizeof(int));

            AnimationFileWrite.write(reinterpret_cast<const char*>(&it.m_Bones[i].m_LocalTransform), sizeof(glm::mat4));
            AnimationFileWrite.write(reinterpret_cast<const char*>(&it.m_Bones[i].BoneName), sizeof(it.m_Bones[i].BoneName));
        }

        AnimationFileWrite.write(reinterpret_cast<const char*>(&it.m_RootNode.transformation), sizeof(glm::mat4));
        AnimationFileWrite.write(reinterpret_cast<const char*>(&it.m_RootNode.name), sizeof(it.m_RootNode.name));
        AnimationFileWrite.write(reinterpret_cast<const char*>(&it.m_RootNode.childrenCount), sizeof(int));

        for (unsigned int i = 0; i < it.m_RootNode.childrenCount; ++i)
        {
            std::cout << "ChildrenCount: " << it.m_RootNode.children[i].childrenCount << std::endl;
        }

        //int nodeChildrenSize = it.m_RootNode.children.size();
        //AnimationFileWrite.write(reinterpret_cast<const char*>(&nodeChildrenSize), sizeof(nodeChildrenSize));
        //AnimationFileWrite.write(reinterpret_cast<const char*>(it.m_RootNode.children.data()), sizeof(AssimpNodeData) * nodeChildrenSize);       
    
    }

    AnimationFileWrite.close();
    // std::cout << "Done Writing to Animation File " << std::endl;
}

void EclipseCompiler::AnimationCompiler::ReleaseFile()
{
    WriteToFile(Animation);
}

void EclipseCompiler::AnimationCompiler::ReadFile(std::string& in)
{

}

void EclipseCompiler::AnimationCompiler::ReadFile()
{
}
