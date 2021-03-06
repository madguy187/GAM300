#pragma once
#include "ASSIMP/include/assimp/Importer.hpp"
#include "ASSIMP/include/assimp/scene.h"
#include "ASSIMP/include/assimp/postprocess.h"
#include "ASSIMP/include/assimp/mesh.h"
#include "ASSIMP/include/assimp/quaternion.h"
#include "ASSIMP/include/assimp/matrix4x4.h"

#define MAX_BONE_INFLUENCE 4

namespace Eclipse
{
    class mTexture
    {
    public:
        mTexture() {}

        mTexture(std::string path) :
            TexturePath(path)
        {

        }

        mTexture(std::string dir, std::string path, aiTextureType type) :
            TextureDirectory(dir),
            TexturePath(path),
            Type(type)
        {

        }

        aiTextureType Type{ aiTextureType_NONE };
        std::string TextureDirectory;
        std::string TexturePath;
    };

    struct mVertex
    {
        glm::vec3 Position{ 0,0,0 };
        glm::vec3 Normal{ 0,0,0 };
        glm::vec2 TextureCoodinates{ 0,0 };
        glm::vec3 Tangents{ 0,0,0 };
        glm::vec4 m_Color{ 0,0,0,0 };
        int m_BoneIDs[MAX_BONE_INFLUENCE] = { 0 };
        float m_Weights[MAX_BONE_INFLUENCE] = { 0.0f };
    };

    struct mMesh
    {
    public:
        glm::vec4 Diffuse{ 0,0,0,0 };
        glm::vec4 Specular{ 0,0,0,0 };
        glm::vec4 Ambient{ 0,0,0,0 };
        std::array<char, 128> MeshName;
        bool NoTex{ true };
        std::vector<mVertex> Vertices;
        std::vector<unsigned int> Indices;
        std::vector<mTexture> Textures;
        unsigned int VBO = 0;
        unsigned int VAO = 0;
        unsigned int EBO = 0;

        mMesh()
        {
            strcpy_s(MeshName.data(), MeshName.size(), "Mesh");
            MeshName[MeshName.size() - 1] = '\0';
        };

        mMesh(std::vector<mVertex> vertices, std::vector<unsigned int> indices, std::string namein, std::vector<mTexture> textures)
            :
            Vertices(vertices),
            Indices(indices),
            Textures(textures),
            NoTex(false)
        {
            //std::cout << "MeshName: " << namein << std::endl;
            strcpy_s(MeshName.data(), MeshName.size(), namein.data());
            MeshName[MeshName.size() - 1] = '\0';
        }

        mMesh(std::vector<mVertex> vertices, std::vector<unsigned int> indices, glm::vec4  diffuse, glm::vec4  specular, glm::vec4  ambient, bool in, std::string namein)
            :
            Vertices(vertices),
            Indices(indices),
            Diffuse(diffuse),
            Specular(specular),
            Ambient(ambient),
            NoTex(in)
        {
            //std::cout << "MeshName: " << namein << std::endl;
            strcpy_s(MeshName.data(), MeshName.size(), namein.data());
            MeshName[MeshName.size() - 1] = '\0';
        }

        ~mMesh() {};
    };

    struct mMeshData
    {
        std::vector<mVertex> vertices;
        std::vector<unsigned int> indices;
        glm::vec4 Diffuse{ 0.0f, 0.0f, 0.0f, 0.0f };
        glm::vec4 Specular{ 0.0f, 0.0f, 0.0f, 0.0f };
        glm::vec4 Ambient{ 0.0f, 0.0f, 0.0f, 0.0f };
        bool NoTextures = false;
        std::string MeshName{};
        std::vector<mTexture> textures;
    };

    struct AssimpGLMHelpers
    {
        static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
        {
            glm::mat4 to;

            to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
            to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
            to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
            to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;

            return to;
        }

        static inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
        {
            return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
        }

        static inline glm::vec3 GetGLMVec(const aiVector3D& vec)
        {
            return glm::vec3(vec.x, vec.y, vec.z);
        }
    };

    struct mBoneInfo
    {
        int id;
        glm::mat4 offset;
        std::array<char, 128> name;
    };

    struct mKeyPosition
    {
        glm::vec3 position;
        float timeStamp;
    };

    struct mKeyRotation
    {
        glm::quat orientation;
        float timeStamp;
    };

    struct mKeyScale
    {
        glm::vec3 scale;
        float timeStamp;
    };

    struct mBone
    {
        std::vector<mKeyPosition> m_Positions;
        std::vector<mKeyRotation> m_Rotations;
        std::vector<mKeyScale> m_Scales;

        int m_NumPositions;
        int m_NumRotations;
        int m_NumScalings;

        int m_ID;
        glm::mat4 m_LocalTransform;
        std::array<char, 128> BoneName;

        mBone(const std::string& name, int ID, const aiNodeAnim* channel)
            :
            m_ID(ID),
            m_LocalTransform(1.0f)
        {
            strcpy_s(BoneName.data(), BoneName.size(), name.data());
            BoneName[BoneName.size() - 1] = '\0';

            m_NumPositions = channel->mNumPositionKeys;

            for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
            {
                aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
                float timeStamp = static_cast<float>(channel->mPositionKeys[positionIndex].mTime);
                mKeyPosition data;
                data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
                data.timeStamp = timeStamp;
                m_Positions.push_back(data);
            }

            m_NumRotations = channel->mNumRotationKeys;
            for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
            {
                aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
                float timeStamp = static_cast<float>(channel->mRotationKeys[rotationIndex].mTime);
                mKeyRotation data;
                data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
                data.timeStamp = timeStamp;
                m_Rotations.push_back(data);
            }

            m_NumScalings = channel->mNumScalingKeys;
            for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
            {
                aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
                float timeStamp = static_cast<float>(channel->mScalingKeys[keyIndex].mTime);
                mKeyScale data;
                data.scale = AssimpGLMHelpers::GetGLMVec(scale);
                data.timeStamp = timeStamp;
                m_Scales.push_back(data);
            }
        }
    };

    struct mAssimpNodeData
    {
        glm::mat4 transformation{ 0 };
        std::array<char, 128> name{};
        int childrenCount = 0;
        std::vector<mAssimpNodeData> children;
    };

    struct mAnimationData
    {
        float m_Duration = 0.0f;
        int m_TicksPerSecond = 0;
        std::array<char, 128> modelName{};
        std::vector<mBoneInfo> m_BoneInfo;
        std::vector<mBone> m_Bones;
        mAssimpNodeData m_RootNode;
        //std::map<std::string, BoneInfo> m_BoneInfoMap;
    };

    class AssimpAnimator
    {
    public:
        unsigned int MeshIndex = 0;
        std::string NameOfModel;
        std::string Directory;
        std::vector<mMesh> Meshes;
        std::vector<glm::vec3> AllVertices;
        std::vector<mMeshData> meshData;
        std::vector<mTexture> Textures_loaded;
        std::map <std::string, std::map<std::string, mBoneInfo>> AllBoneInfoMaps;
        std::map <std::string, int> AllBoneCount;
        std::vector<mAnimationData> AnimationContainer;
        std::unordered_map<std::string, mMesh> GeometryContainer;
    public:

        void LoadAssimpModel(std::string path);
        void ProcessGeometry(aiNode* node, const aiScene* scene);
        void ProcessMesh(aiMesh* mesh, const aiScene* scene, const char* MeshName);
        float GetLargestAxisValue(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        void ComputeAxisMinMax(std::vector<glm::vec3>& vertices, std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        glm::vec3 ComputeCentroid(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        void LoadNewModel(std::unordered_map<std::string, mMesh>& GeometryContainer);
        void SetVertexBoneDataToDefault(mVertex& vertex);
        void SetVertexBoneData(mVertex& vertex, int boneID, float weight);
        void ExtractBoneWeightForVertices(std::vector<mVertex>& vertices, aiMesh* mesh, const aiScene* scene, const char* MeshName);
    public:
        void LoadAssimpModelForTextures(std::string path, std::vector < std::pair<std::string, mTexture>>&);
        void ProcessTextures(aiNode* node, const aiScene* scene, std::vector < std::pair<std::string, mTexture>>& TextureContainer);
        std::vector<mTexture> ExtractTextures(aiMesh* mesh, const aiScene* scene, std::string& MeshName, std::vector < std::pair<std::string, mTexture>>& TextureContainer);
        std::vector<mTexture> LoadTextures(aiMaterial* mat, aiTextureType type);
        std::vector<mTexture> LoadTexturesForCompiler(aiMaterial* mat, aiTextureType type, std::string& MeshName, std::vector < std::pair<std::string, mTexture>>&);

    public:
        //Animation
        void LoadAssimpAnimationModel(std::string path);
        void LoadNewAnimationModel();
        void LoadAnimationData(std::string path);
        void ReadHeirarchyData(mAssimpNodeData& dest, const aiNode* src);
        void SetupBones(mAnimationData& animationData, const aiAnimation* animation, const aiNode* node);

        void LoadAnimationFile(const std::string& animationFile);
    };
}