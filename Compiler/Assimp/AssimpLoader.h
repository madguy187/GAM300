#pragma once
#include "ASSIMP/include/assimp/Importer.hpp"
#include "ASSIMP/include/assimp/scene.h"
#include "ASSIMP/include/assimp/postprocess.h"
#include "ASSIMP/include/assimp/mesh.h"
#include "ASSIMP/include/assimp/quaternion.h"
#include "ASSIMP/include/assimp/matrix4x4.h"

#define MAX_BONE_INFLUENCE 4

namespace EclipseCompiler
{
    class Texture
    {
    public:
        Texture() {}

        Texture(std::string path) :
            TexturePath(path)
        {

        }

        Texture(std::string dir, std::string path, aiTextureType type) :
            TextureDirectory(dir),
            TexturePath(path),
            Type(type)
        {

        }

        aiTextureType Type;
        std::string TextureDirectory;
        std::string TexturePath;
    };

    struct Vertex
    {
        glm::vec3 Position{ 0,0,0 };
        glm::vec3 Normal{ 0,0,0 };
        glm::vec2 TextureCoodinates{ 0,0 };
        glm::vec3 Tangents{ 0,0,0 };
        glm::vec4 m_Color{ 0,0,0,0 };
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct Mesh
    {
    public:
        glm::vec4 Diffuse{ 0,0,0,0 };
        glm::vec4 Specular{ 0,0,0,0 };
        glm::vec4 Ambient{ 0,0,0,0 };
        std::array<char, 128> MeshName;
        bool NoTex{ true };
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
        std::vector<Texture> Textures;
        unsigned int VBO = 0;
        unsigned int VAO = 0;
        unsigned int EBO = 0;

        Mesh()
        {
            strcpy_s(MeshName.data(), MeshName.size(), "Mesh");
            MeshName[MeshName.size() - 1] = '\0';
        };

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string namein, std::vector<Texture> textures)
            :
            Vertices(vertices),
            Indices(indices),
            Textures(textures),
            NoTex(false)
        {
            strcpy_s(MeshName.data(), MeshName.size(), namein.data());
            MeshName[MeshName.size() - 1] = '\0';
        }

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::vec4  diffuse, glm::vec4  specular, glm::vec4  ambient, bool in, std::string namein)
            :
            Vertices(vertices),
            Indices(indices),
            Diffuse(diffuse),
            Specular(specular),
            Ambient(ambient),
            NoTex(in)
        {
            strcpy_s(MeshName.data(), MeshName.size(), namein.data());
            MeshName[MeshName.size() - 1] = '\0';
        }

        ~Mesh() {};
    };

    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        glm::vec4 Diffuse;
        glm::vec4 Specular;
        glm::vec4 Ambient;
        bool NoTextures = false;
        const char* MeshName;
        std::vector<Texture> textures;
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

    struct BoneInfo
    {
        int id;
        glm::mat4 offset;
        std::array<char, 128> name;
    };

    struct KeyPosition
    {
        glm::vec3 position;
        float timeStamp;
    };

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

        Bone(const std::string& name, int ID, const aiNodeAnim* channel)
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
                float timeStamp = channel->mPositionKeys[positionIndex].mTime;
                KeyPosition data;
                data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
                data.timeStamp = timeStamp;
                m_Positions.push_back(data);
            }

            m_NumRotations = channel->mNumRotationKeys;
            for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
            {
                aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
                float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
                KeyRotation data;
                data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
                data.timeStamp = timeStamp;
                m_Rotations.push_back(data);
            }

            m_NumScalings = channel->mNumScalingKeys;
            for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
            {
                aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
                float timeStamp = channel->mScalingKeys[keyIndex].mTime;
                KeyScale data;
                data.scale = AssimpGLMHelpers::GetGLMVec(scale);
                data.timeStamp = timeStamp;
                m_Scales.push_back(data);
            }
        }
    };

    struct AssimpNodeData
    {
        glm::mat4 transformation;
        std::array<char, 128> name;
        int childrenCount;
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

    class AssimpLoader
    {
    public:
        unsigned int MeshIndex = 0;
        std::string NameOfModel;
        std::string Directory;
        std::vector<Mesh> Meshes;
        std::vector<glm::vec3> AllVertices;
        std::vector<MeshData> meshData;
        std::vector<Texture> Textures_loaded;
        std::map <std::string, std::map<std::string, BoneInfo>> AllBoneInfoMaps;
        std::map <std::string, int> AllBoneCount;
    public:

        void LoadAssimpModel(std::string path, std::unordered_map<std::string, Mesh>& GeometryContainer);
        void ProcessGeometry(aiNode* node, const aiScene* scene);
        void ProcessMesh(aiMesh* mesh, const aiScene* scene, const char* MeshName);
        float GetLargestAxisValue(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        void ComputeAxisMinMax(std::vector<glm::vec3>& vertices, std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        glm::vec3 ComputeCentroid(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        void LoadNewModel(std::unordered_map<std::string, Mesh>& GeometryContainer, bool HasAnimation);
        void SetVertexBoneDataToDefault(Vertex& vertex);
        void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
        void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene, const char* MeshName);
    public:
        void LoadAssimpModelForTextures(std::string path, std::vector < std::pair<std::string, Texture>>&);
        void ProcessTextures(aiNode* node, const aiScene* scene, std::vector < std::pair<std::string, Texture>>& TextureContainer);
        std::vector<Texture> ExtractTextures(aiMesh* mesh, const aiScene* scene, std::string& MeshName, std::vector < std::pair<std::string, Texture>>& TextureContainer);
        std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type);
        std::vector<Texture> LoadTexturesForCompiler(aiMaterial* mat, aiTextureType type, std::string& MeshName, std::vector < std::pair<std::string, Texture>>&);

    public:
        //Animation
        void LoadAssimpAnimationModel(std::string path);
        void LoadNewAnimationModel();
        void LoadAnimationData(std::string path, std::vector<AnimationData>& AnimationContainer);
        void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
        void SetupBones(AnimationData& animationData, const aiAnimation* animation, const aiNode* node);
    };
}