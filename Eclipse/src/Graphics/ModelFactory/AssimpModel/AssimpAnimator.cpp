#include "pch.h"
#include "AssimpAnimator.h"

namespace Eclipse
{
    void AssimpAnimator::LoadAssimpModel(std::string path)
    {
        unsigned int importOptions = aiProcess_Triangulate | aiProcess_GenSmoothNormals
            | aiProcess_CalcTangentSpace | aiProcess_FlipWindingOrder
            | aiProcess_TransformUVCoords;

        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, importOptions);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::string Error = ("Compiler Could not load " + path + import.GetErrorString()).c_str();
            return;
        }

        if (!scene->HasAnimations())
        {
            importOptions =
                aiProcess_Triangulate |
                aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs |
                aiProcess_JoinIdenticalVertices |
                aiProcess_FlipWindingOrder |
                aiProcess_RemoveRedundantMaterials |
                aiProcess_FindDegenerates |
                aiProcess_FindInvalidData |
                aiProcess_GenUVCoords |
                aiProcess_TransformUVCoords |
                aiProcess_FindInstances |
                aiProcess_PreTransformVertices |
                aiProcess_CalcTangentSpace;

            scene = import.ReadFile(path, importOptions);
        }

        Directory = path.substr(0, path.find_last_of("/"));

        ProcessGeometry(scene->mRootNode, scene);
        LoadNewModel(GeometryContainer);
    }

    void AssimpAnimator::LoadAssimpModelForTextures(std::string path, std::vector < std::pair<std::string, mTexture>>& textureContainer)
    {
        unsigned int importOptions = aiProcess_Triangulate | aiProcess_GenSmoothNormals
            | aiProcess_CalcTangentSpace | aiProcess_FlipWindingOrder
            | aiProcess_TransformUVCoords;

        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, importOptions);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::string Error = ("Compiler Could not load " + path + import.GetErrorString()).c_str();
            return;
        }

        if (!scene->HasAnimations())
        {
            importOptions =
                aiProcess_Triangulate |
                aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs |
                aiProcess_JoinIdenticalVertices |
                aiProcess_FlipWindingOrder |
                aiProcess_RemoveRedundantMaterials |
                aiProcess_FindDegenerates |
                aiProcess_FindInvalidData |
                aiProcess_GenUVCoords |
                aiProcess_TransformUVCoords |
                aiProcess_FindInstances |
                aiProcess_PreTransformVertices |
                aiProcess_CalcTangentSpace;

            scene = import.ReadFile(path, importOptions);
        }

        Directory = path.substr(0, path.find_last_of("/"));
        ProcessTextures(scene->mRootNode, scene, textureContainer);
    }

    void AssimpAnimator::ProcessGeometry(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            const char* NodeName = node->mName.data;
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene, NodeName);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessGeometry(node->mChildren[i], scene);
        }
    }

    void AssimpAnimator::ProcessTextures(aiNode* node, const aiScene* scene, std::vector < std::pair<std::string, mTexture>>& TextureContainer)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            std::string NodeName = node->mName.data;
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ExtractTextures(mesh, scene, NodeName, TextureContainer);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessTextures(node->mChildren[i], scene, TextureContainer);
        }
    }

    std::vector<mTexture> AssimpAnimator::ExtractTextures(aiMesh* mesh, const aiScene* scene, std::string& MeshName, std::vector <std::pair<std::string, mTexture>>& TextureContainer)
    {
        std::vector<mTexture> textures;

        // process material
        if (mesh->mMaterialIndex >= 0 && scene->HasMaterials())
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            aiString texture_file;
            material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);

            // diffuse maps
            std::vector<mTexture> diffuseMaps = LoadTexturesForCompiler(material, aiTextureType_DIFFUSE, MeshName, TextureContainer);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // specular maps
            std::vector<mTexture> specularMaps = LoadTexturesForCompiler(material, aiTextureType_SPECULAR, MeshName, TextureContainer);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            std::vector<mTexture> normalMaps = LoadTexturesForCompiler(material, aiTextureType_NORMALS, MeshName, TextureContainer);
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        }

        MeshIndex++;
        return textures;
    }

    void AssimpAnimator::ProcessMesh(aiMesh* mesh, const aiScene* scene, const char* MeshName)
    {
        mMeshData newMesh;
        std::vector<mVertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<mTexture> textures;

        // Set Mesh Name
        newMesh.MeshName = MeshName;

        // vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            mVertex vertex;

            SetVertexBoneDataToDefault(vertex);

            // position
            vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            // normal vectors
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

            // textures
            if (mesh->mTextureCoords[0])
            {
                vertex.TextureCoodinates = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            else
            {
                vertex.TextureCoodinates = glm::vec2(0.0f);
            }

            if (mesh->mColors[0])
            {
                vertex.m_Color.x = mesh->mColors[0][i].r;
                vertex.m_Color.y = mesh->mColors[0][i].g;
                vertex.m_Color.z = mesh->mColors[0][i].b;
                vertex.m_Color.w = mesh->mColors[0][i].a;
            }

            // tangent vector
            vertex.Tangents = {
                mesh->mTangents[i].x,
                mesh->mTangents[i].y,
                mesh->mTangents[i].z
            };

            AllVertices.push_back(vertex.Position);
            vertices.push_back(vertex);
            newMesh.vertices.push_back(vertex);
        }

        // process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
                newMesh.indices.push_back(face.mIndices[j]);
            }
        }

        // process material
        if (mesh->mMaterialIndex >= 0 && scene->HasMaterials())
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            aiString texture_file;
            material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);

            newMesh.NoTextures = false;

            // diffuse maps
            std::vector<mTexture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            newMesh.textures.insert(newMesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // specular maps
            std::vector<mTexture> specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            newMesh.textures.insert(newMesh.textures.end(), specularMaps.begin(), specularMaps.end());

            std::vector<mTexture> normalMaps = LoadTextures(material, aiTextureType_NORMALS);
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

            // If no textures are found , we get the material
            if (newMesh.textures.empty())
            {
                // If no textures are found , set to false
                newMesh.NoTextures = true;

                // diffuse color
                aiColor4D diff(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);
                newMesh.Diffuse.x = diff.r;
                newMesh.Diffuse.y = diff.g;
                newMesh.Diffuse.z = diff.b;
                newMesh.Diffuse.w = diff.a;

                // specular color
                aiColor4D spec(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);
                newMesh.Specular.x = spec.r;
                newMesh.Specular.y = spec.g;
                newMesh.Specular.z = spec.b;
                newMesh.Specular.w = spec.a;

                // specular color
                aiColor4D ambientColor(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
                newMesh.Ambient.x = ambientColor.r;
                newMesh.Ambient.y = ambientColor.g;
                newMesh.Ambient.z = ambientColor.b;
                newMesh.Ambient.w = ambientColor.a;

                meshData.push_back(newMesh);

                MeshIndex++;
                return;
            }
        }

        ExtractBoneWeightForVertices(newMesh.vertices, mesh, scene, MeshName);
        meshData.push_back(newMesh);

        MeshIndex++;
        return;
    }

    std::vector<mTexture> AssimpAnimator::LoadTextures(aiMaterial* mat, aiTextureType type)
    {
        std::vector<mTexture> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            // prevent duplicate loading
            bool skip = false;
            for (unsigned int j = 0; j < Textures_loaded.size(); j++)
            {
                if (std::strcmp(Textures_loaded[j].TexturePath.data(), str.C_Str()) == 0)
                {
                    textures.push_back(Textures_loaded[j]);
                    std::unique_ptr<mTexture> ptr(new mTexture(Textures_loaded[j]));

                    //Textures[MeshName][MeshId].push_back(std::move(in));

                    skip = true;
                    break;
                }
            }

            if (!skip)
            {
                mTexture tex(Directory, str.C_Str(), type);
                textures.push_back(tex);
                Textures_loaded.push_back(tex);
            }
        }
        return textures;
    }

    std::vector<mTexture> AssimpAnimator::LoadTexturesForCompiler(aiMaterial* mat, aiTextureType type, std::string& MeshName, std::vector < std::pair<std::string, mTexture>>& In)
    {
        std::vector<mTexture> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            // prevent duplicate loading
            bool skip = false;
            for (unsigned int j = 0; j < Textures_loaded.size(); j++)
            {
                if (std::strcmp(Textures_loaded[j].TexturePath.data(), str.C_Str()) == 0)
                {
                    textures.push_back(Textures_loaded[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip)
            {
                mTexture tex(Directory, str.C_Str(), type);
                textures.push_back(tex);
                Textures_loaded.push_back(tex);
                In.push_back({ MeshName,tex });
            }
        }

        return textures;
    }

    void AssimpAnimator::LoadAssimpAnimationModel(std::string path)
    {
        unsigned int importOptions = aiProcess_Triangulate | aiProcess_GenSmoothNormals
            | aiProcess_CalcTangentSpace | aiProcess_FlipWindingOrder
            | aiProcess_TransformUVCoords;

        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, importOptions);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode || !scene->HasAnimations())
        {
            std::string Error = ("Compiler Could not load " + path + import.GetErrorString()).c_str();
            return;
        }
      
        Directory = path.substr(0, path.find_last_of("/"));

        ProcessGeometry(scene->mRootNode, scene);
        LoadNewAnimationModel();
    }

    void AssimpAnimator::LoadNewAnimationModel()
    {
        std::vector<glm::vec3> combinedVertices;

        for (auto& it : meshData)
        {
            for (auto& vertex : it.vertices)
            {
                combinedVertices.push_back(vertex.Position);
            }
        }

        std::pair<float, float> minmaxX;
        std::pair<float, float> minmaxY;
        std::pair<float, float> minmaxZ;

        ComputeAxisMinMax(combinedVertices, minmaxX, minmaxY, minmaxZ);

        glm::vec3 centroid = ComputeCentroid(minmaxX, minmaxY, minmaxZ);
        float largestAxis = GetLargestAxisValue(minmaxX, minmaxY, minmaxZ);

        for (auto& it : meshData)
        {
            for (auto& vertex : it.vertices)
            {
                vertex.Position -= centroid;

                vertex.Position.x /= largestAxis;
                vertex.Position.y /= largestAxis;
                vertex.Position.z /= largestAxis;
            }

            if (it.NoTextures == false)
            {
                mMesh NewMesh(it.vertices, it.indices, it.MeshName, it.textures);
                Meshes.push_back(NewMesh);
            }
            else
            {
                mMesh NewMesh(it.vertices, it.indices, it.Diffuse, it.Specular, it.Ambient, it.NoTextures, it.MeshName);
                Meshes.push_back(NewMesh);
            }
        }

        for (auto& it : AllVertices)
        {
            it -= centroid;

            it.x /= largestAxis;
            it.y /= largestAxis;
            it.z /= largestAxis;
        }
    }

    void AssimpAnimator::LoadAnimationData(std::string path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode || !scene->HasAnimations())
        {
            std::string Error = ("Compiler could not load animation data at " + path + importer.GetErrorString()).c_str();
            return;
        }

        Directory = path.substr(0, path.find_last_of("/"));

        mAnimationData newAnimation;

        auto animation = scene->mAnimations[0];

        const char* NodeName = " ";

        for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; i++)
        {
            if (scene->mRootNode->mChildren[i]->mNumMeshes > 0)
            {
                NodeName = scene->mRootNode->mChildren[i]->mName.C_Str();
            }
        }

        strcpy_s(newAnimation.modelName.data(), newAnimation.modelName.size(), NodeName);
        newAnimation.modelName[newAnimation.modelName.size() - 1] = '\0';
        newAnimation.m_Duration = static_cast<float>(animation->mDuration);
        newAnimation.m_TicksPerSecond = static_cast<int>(animation->mTicksPerSecond);

        aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
        globalTransformation = globalTransformation.Inverse();
        ReadHeirarchyData(newAnimation.m_RootNode, scene->mRootNode);
        SetupBones(newAnimation, animation, scene->mRootNode);

        AnimationContainer.push_back(newAnimation);
    }

    void AssimpAnimator::ReadHeirarchyData(mAssimpNodeData& dest, const aiNode* src)
    {
        assert(src);

        strcpy_s(dest.name.data(), dest.name.size(), src->mName.C_Str());
        dest.name[dest.name.size() - 1] = '\0';

        dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
        dest.childrenCount = src->mNumChildren;

        for (int i = 0; i < static_cast<int>(src->mNumChildren); i++)
        {
            mAssimpNodeData newData;
            ReadHeirarchyData(newData, src->mChildren[i]);
            dest.children.push_back(newData);
        }
    }

    void AssimpAnimator::SetupBones(mAnimationData& animationData, const aiAnimation* animation, const aiNode* node)
    {
        (void)node;

        int size = animation->mNumChannels;

        std::string meshName = std::string(animationData.modelName.data());

        auto& boneInfoMap = AllBoneInfoMaps[meshName];
        int& boneCount = AllBoneCount[meshName];

        for (int i = 0; i < size; i++)
        {
            auto channel = animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;

            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                strcpy_s(boneInfoMap[boneName].name.data(), boneInfoMap[boneName].name.size(), boneName.c_str());
                boneInfoMap[boneName].name[boneInfoMap[boneName].name.size() - 1] = '\0';

                boneInfoMap[boneName].id = boneCount;
                boneCount++;
            }

            animationData.m_Bones.push_back(mBone(channel->mNodeName.data,
                boneInfoMap[channel->mNodeName.data].id, channel));
        }

        //m_BoneInfoMap = boneInfoMap;
        for (auto& it : boneInfoMap)
        {
            animationData.m_BoneInfo.push_back(it.second);
        }
    }

    void AssimpAnimator::LoadAnimationFile(const std::string& animationFile)
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
    
            for (auto& _dirEntry : std::filesystem::directory_iterator(GoIntoModelFolder))
            {
                const auto& FbxOrGltf = _dirEntry.path();
                auto relativePathIn = relative(FbxOrGltf, "..//Eclipse//src//");
                std::string FbxOrGltfName = relativePathIn.filename().string();
    
                if (FbxOrGltfName.find("gltf") != std::string::npos || FbxOrGltfName.find("fbx") != std::string::npos)
                {
                    std::string PathName = ("..//Eclipse//src/Assets/Models/" + FolderName + "/" + FbxOrGltfName).c_str();

                    LoadAssimpAnimationModel(PathName);
                    LoadAnimationData(PathName);
                }
            }
        }
    }

    float AssimpAnimator::GetLargestAxisValue(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
    {
        float minValue = (std::min)({ _minmaxX.first, _minmaxY.first, _minmaxZ.first });
        float maxValue = (std::max)({ _minmaxX.second, _minmaxY.second, _minmaxZ.second });

        float largestAxis = (std::max)(std::abs(minValue), maxValue);

        return largestAxis;
    }

    void AssimpAnimator::ComputeAxisMinMax(std::vector<glm::vec3>& vertices, std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
    {
        std::vector<float> verticesX;
        std::vector<float> verticesY;
        std::vector<float> verticesZ;

        for (auto& it : vertices)
        {
            verticesX.push_back(it.x);
            verticesY.push_back(it.y);
            verticesZ.push_back(it.z);
        }

        auto minmaxX = std::minmax_element(verticesX.begin(), verticesX.end());
        auto minmaxY = std::minmax_element(verticesY.begin(), verticesY.end());
        auto minmaxZ = std::minmax_element(verticesZ.begin(), verticesZ.end());

        _minmaxX.first = *minmaxX.first;
        _minmaxX.second = *minmaxX.second;

        _minmaxY.first = *minmaxY.first;
        _minmaxY.second = *minmaxY.second;

        _minmaxZ.first = *minmaxZ.first;
        _minmaxZ.second = *minmaxZ.second;
    }

    glm::vec3 AssimpAnimator::ComputeCentroid(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
    {
        glm::vec3 centroid = glm::vec3{
            (_minmaxX.first + _minmaxX.second) / 2.0f,
            (_minmaxY.first + _minmaxY.second) / 2.0f,
            (_minmaxZ.first + _minmaxZ.second) / 2.0f };

        return centroid;
    }

    void AssimpAnimator::LoadNewModel(std::unordered_map<std::string, mMesh>& _GeometryContainer)
    {
        std::vector<glm::vec3> combinedVertices;

        for (auto& it : meshData)
        {
            for (auto& vertex : it.vertices)
            {
                combinedVertices.push_back(vertex.Position);
            }
        }

        std::pair<float, float> minmaxX;
        std::pair<float, float> minmaxY;
        std::pair<float, float> minmaxZ;

        ComputeAxisMinMax(combinedVertices, minmaxX, minmaxY, minmaxZ);

        glm::vec3 centroid = ComputeCentroid(minmaxX, minmaxY, minmaxZ);
        float largestAxis = GetLargestAxisValue(minmaxX, minmaxY, minmaxZ);

        for (auto& it : meshData)
        {
            for (auto& vertex : it.vertices)
            {
                vertex.Position -= centroid;

                vertex.Position.x /= largestAxis;
                vertex.Position.y /= largestAxis;
                vertex.Position.z /= largestAxis;
            }

            if (it.NoTextures == false)
            {
                mMesh NewMesh(it.vertices, it.indices, it.MeshName, it.textures);
                Meshes.push_back(NewMesh);
                _GeometryContainer.emplace(it.MeshName, (NewMesh));
            }
            else
            {
                mMesh NewMesh(it.vertices, it.indices, it.Diffuse, it.Specular, it.Ambient, it.NoTextures, it.MeshName);
                Meshes.push_back(NewMesh);
                _GeometryContainer.emplace(it.MeshName, (NewMesh));
            }
        }

        for (auto& it : AllVertices)
        {
            it -= centroid;

            it.x /= largestAxis;
            it.y /= largestAxis;
            it.z /= largestAxis;
        }
    }

    void AssimpAnimator::SetVertexBoneDataToDefault(mVertex& vertex)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            vertex.m_BoneIDs[i] = -1;
            vertex.m_Weights[i] = 0.0f;
        }
    }

    void AssimpAnimator::SetVertexBoneData(mVertex& vertex, int boneID, float weight)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        {
            if (vertex.m_BoneIDs[i] < 0)
            {
                vertex.m_Weights[i] = weight;
                vertex.m_BoneIDs[i] = boneID;
                break;
            }
        }
    }

    void AssimpAnimator::ExtractBoneWeightForVertices(std::vector<mVertex>& vertices, aiMesh* mesh, const aiScene* scene, const char* MeshName)
    {
        (void)scene;

        std::string meshName = std::string(MeshName);

        auto& boneInfoMap = AllBoneInfoMaps[meshName];
        int& boneCount = AllBoneCount[meshName];

        for (int boneIndex = 0; boneIndex < static_cast<int>(mesh->mNumBones); ++boneIndex)
        {
            int boneID = -1;
            std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                mBoneInfo newBoneInfo;

                // Set values of new boneinfo
                newBoneInfo.id = boneCount;
                newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
                strcpy_s(newBoneInfo.name.data(), newBoneInfo.name.size(), boneName.data());
                newBoneInfo.name[newBoneInfo.name.size() - 1] = '\0';

                boneInfoMap[boneName] = newBoneInfo;
                boneID = boneCount;
                boneCount++;
            }
            else
            {
                boneID = boneInfoMap[boneName].id;
            }

            assert(boneID != -1);
            auto weights = mesh->mBones[boneIndex]->mWeights;
            int numWeights = mesh->mBones[boneIndex]->mNumWeights;

            for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                int vertexId = weights[weightIndex].mVertexId;
                float weight = weights[weightIndex].mWeight;
                assert(vertexId <= vertices.size());
                SetVertexBoneData(vertices[vertexId], boneID, weight);
            }
        }
    }
}
