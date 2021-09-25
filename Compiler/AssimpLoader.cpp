#include "pch.h"
#include "AssimpLoader.h"

#include <filesystem>

namespace EclipseCompiler
{
    void AssimpLoader::LoadModels(const std::string& modelFile)
    {
        for (auto& dirEntry : std::filesystem::directory_iterator(modelFile))
        {
            const auto& path = dirEntry.path();
            auto relativePath = relative(path, "..//Eclipse//src//");
            std::string FolderName = relativePath.filename().string();

            std::string GoIntoModelFolder = ("..//Eclipse//src/Assets/ASSModels/" + FolderName);

            for (auto& dirEntry : std::filesystem::directory_iterator(GoIntoModelFolder))
            {
                const auto& FbxOrGltf = dirEntry.path();
                auto relativePath = relative(FbxOrGltf, "..//Eclipse//src//");
                std::string FbxOrGltfName = relativePath.filename().string();

                if (FbxOrGltfName.find("gltf") != std::string::npos || FbxOrGltfName.find("fbx") != std::string::npos)
                {
                    std::string PathName = ("..//Eclipse//src/Assets/ASSModels/" + FolderName + "/" + FbxOrGltfName).c_str();
                    ModelMap.emplace(FolderName, PathName);
                }
            }
        }
    }

    void AssimpLoader::LoadAssimpModel(std::string path)
    {
        unsigned int importOptions =
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
            aiProcess_PreTransformVertices;

        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, importOptions);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::string Error = ("Compiler Could not load " + path + import.GetErrorString()).c_str();
            return;
        }

        Directory = path.substr(0, path.find_last_of("/"));
        ProcessNode(scene->mRootNode, scene);
        LoadNewModel();
    }

    void AssimpLoader::ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            std::string NodeName = node->mName.data;
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene, NodeName);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    void AssimpLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string& MeshName)
    {
        MeshData newMesh;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // Set Mesh Name
        newMesh.MeshName = MeshName;

        // vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;

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
                vertex.m_Color = mesh->mColors[0][i];
            }

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
            std::vector<Texture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE, newMesh.MeshName);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            newMesh.textures.insert(newMesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // specular maps
            std::vector<Texture> specularMaps = LoadTextures(material, aiTextureType_SPECULAR, newMesh.MeshName);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            newMesh.textures.insert(newMesh.textures.end(), specularMaps.begin(), specularMaps.end());

            // If no textures are found , we get the material
            if (newMesh.textures.empty())
            {
                // If no textures are found , set to false
                newMesh.NoTextures = true;

                // diffuse color
                aiColor4D diff(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);
                newMesh.Diffuse = diff;

                // specular color
                aiColor4D spec(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);
                newMesh.Specular = spec;

                // specular color
                aiColor4D ambientColor(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
                newMesh.Ambient = ambientColor;

                meshData.push_back(newMesh);
                MeshIndex++;
                return;
            }
        }

        meshData.push_back(newMesh);
        MeshIndex++;
        return;
    }

    std::vector<Texture> AssimpLoader::LoadTextures(aiMaterial* mat, aiTextureType type, std::string& MeshName)
    {
        std::vector<Texture> textures;

        // need my own reader for fbx textures
        //Texture tex(directory, "textures/default_Base_Color.png", type);
        //tex.load(false);
        //textures.push_back(tex);
        //textures_loaded.push_back(tex);

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
                    std::unique_ptr<Texture> ptr(new Texture(Textures_loaded[j]));
                    skip = true;
                    break;
                }
            }

            if (!skip)
            {
                Texture tex(Directory, str.C_Str(), type);
                std::cout << "Directory: " << Directory << std::endl;
                std::cout << "Path: " << str.C_Str() << std::endl;
                textures.push_back(tex);
                Textures_loaded.push_back(tex);
            }
        }
        return textures;
    }

    float AssimpLoader::GetLargestAxisValue(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
    {
        float minValue = (std::min)({ _minmaxX.first, _minmaxY.first, _minmaxZ.first });
        float maxValue = (std::max)({ _minmaxX.second, _minmaxY.second, _minmaxZ.second });

        float largestAxis = (std::max)(std::abs(minValue), maxValue);

        return largestAxis;
    }

    void AssimpLoader::ComputeAxisMinMax(std::vector<glm::vec3>& vertices, std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
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

    glm::vec3 AssimpLoader::ComputeCentroid(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
    {
        glm::vec3 centroid = glm::vec3{
            (_minmaxX.first + _minmaxX.second) / 2.0f,
            (_minmaxY.first + _minmaxY.second) / 2.0f,
            (_minmaxZ.first + _minmaxZ.second) / 2.0f };

        return centroid;
    }

    void AssimpLoader::LoadNewModel()
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
                Mesh NewMesh(it.vertices, it.indices, it.MeshName, it.textures);
                Meshes.push_back(NewMesh);
            }
            else
            {
                Mesh NewMesh(it.vertices, it.indices, it.Diffuse, it.Specular, it.Ambient, it.NoTextures, it.MeshName);
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
}
