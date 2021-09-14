#include "pch.h"
#include "AssimpModel/AssimpModel.h"
#include <regex>
#include "Logger/Logger.h"

namespace Eclipse
{

    AssimpModel::AssimpModel(bool noTex)
        //:
        //NoTextures(noTex)
    {
        //GlobalMode = GL_FILL;
    }

    AssimpModel::AssimpModel(bool noTex, std::string& NameOfModels, std::string& Directorys, std::vector<Mesh> Meshess, std::vector<Texture> Textures_loadeds) :
        //NoTextures(noTex),
        NameOfModel(NameOfModels),
        Directory(Directorys),
        Meshes(Meshess),
        Textures_loaded(Textures_loadeds)
    {

    }

    void AssimpModel::Render(Shader& shader, GLenum MOde, unsigned int FrameBufferID, unsigned int id)
    {
        for (unsigned int i = 0; i < Meshes.size(); i++)
        {
            Meshes[i].Render(shader, MOde, id, i);
        }
    }

    void AssimpModel::Cleanup()
    {
        for (unsigned int i = 0; i < Meshes.size(); i++)
        {
            Meshes[i].Cleanup();
        }
    }

    void AssimpModel::LoadAssimpModel(std::string path)
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
            std::string Error = ("Could not load AssimpModel at " + path + import.GetErrorString()).c_str();
            return;
        }

        Directory = path.substr(0, path.find_last_of("/"));

        ProcessNode(scene->mRootNode, scene);

        //TEST CODE
        LoadNewModel();
    }

    void AssimpModel::ProcessNode(aiNode* node, const aiScene* scene)
    {
        // process all meshes
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            std::string NodeName = node->mName.data;
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene, NodeName);
        }

        // process all child nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            //std::cout << NameOfModel << " : " << node->mChildren[i]->mName.data << std::endl;
            ProcessNode(node->mChildren[i], scene);
        }
    }

    float Eclipse::AssimpModel::GetLargestAxisValue(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
    {
        float minValue = (std::min)({ _minmaxX.first, _minmaxY.first, _minmaxZ.first });
        float maxValue = (std::max)({ _minmaxX.second, _minmaxY.second, _minmaxZ.second });

        float largestAxis = (std::max)(std::abs(minValue), maxValue);

        return largestAxis;
    }

    void Eclipse::AssimpModel::ComputeAxisMinMax(std::vector<glm::vec3>& vertices, std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
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

    glm::vec3 Eclipse::AssimpModel::ComputeCentroid(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
    {
        glm::vec3 centroid = glm::vec3{
            (_minmaxX.first + _minmaxX.second) / 2.0f,
            (_minmaxY.first + _minmaxY.second) / 2.0f,
            (_minmaxZ.first + _minmaxZ.second) / 2.0f };

        return centroid;
    }

    void AssimpModel::SetName(std::string& name)
    {
        NameOfModel = name;
    }

    std::string AssimpModel::GetDirectory()
    {
        return Directory;
    }

    unsigned int Eclipse::AssimpModel::GetNumberOfTextures()
    {
        return Textures_loaded.size();
    }

    unsigned int Eclipse::AssimpModel::GetNumberOfMeshes()
    {
        return Meshes.size();
    }

    ModelType Eclipse::AssimpModel::GetType()
    {
        return type;
    }

    void Eclipse::AssimpModel::SetModelType(ModelType in)
    {
        type = in;
    }

    void Eclipse::AssimpModel::LoadNewModel()
    {
        std::vector<glm::vec3> combinedVertices;

        for (auto& it : meshData)
        {
            for (auto& vertex : it.vertices)
            {
                combinedVertices.push_back(vertex.Position);
            }
        }

        //std::cout << "Combined Size: " << combinedVertices.size() << std::endl;

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
                engine->AssimpManager.SingleMeshMap.emplace(it.MeshName, std::make_unique<Mesh>(NewMesh));
            }
            else
            {
                Mesh NewMesh(it.vertices, it.indices, it.Diffuse, it.Specular, it.Ambient, it.NoTextures, it.MeshName);
                Meshes.push_back(NewMesh);
                engine->AssimpManager.SingleMeshMap.emplace(it.MeshName,std::make_unique<Mesh>(NewMesh));
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

    std::vector<glm::vec3> Eclipse::AssimpModel::GetVertices()
    {
        return AllVertices;
    }

    void Eclipse::AssimpModel::SetProperties(std::string& ModelName, ModelType in, unsigned int ID_)
    {
        ID = ID_;
        SetName(ModelName);
        SetModelType(in);
    }

    void Eclipse::AssimpModel::SetProperties(std::string& ModelName, ModelType in)
    {
        SetName(ModelName);
        SetModelType(in);
    }

    unsigned int Eclipse::AssimpModel::GetEntityID()
    {
        return ID;
    }

    std::vector<Mesh> AssimpModel::GetMesh()
    {
        return Meshes;
    }

    std::vector<Texture> AssimpModel::GetTextures()
    {
        return Textures_loaded;
    }

    void Eclipse::AssimpModel::GetTextureNames()
    {
        for (int i = 0; i < Textures_loaded.size(); i++)
        {
            std::cout << " Texture Name " << Textures_loaded[i].GetPath() << std::endl;
        }
    }

    std::string AssimpModel::GetName()
    {
        return NameOfModel;
    }

    void AssimpModel::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string& MeshName)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        //TEST CODE
        MeshData newMesh;

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

            //TEST CODE
            newMesh.vertices.push_back(vertex);
        }


        //std::cout << "Num vertices: " << mesh->mNumVertices << std::endl;

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

            if (auto texture = scene->GetEmbeddedTexture(texture_file.C_Str()))
            {
                std::cout << "Embedded" << std::endl;
                // Not working for fbx file leh
            }
            else
            {
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
        }

        meshData.push_back(newMesh);
        MeshIndex++;
        return;
        // return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> AssimpModel::LoadTextures(aiMaterial* mat, aiTextureType type , std::string& MeshName)
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
                if (std::strcmp(Textures_loaded[j].GetPath().data(), str.C_Str()) == 0)
                {
                    textures.push_back(Textures_loaded[j]);
                    std::unique_ptr<Texture> ptr(new Texture(Textures_loaded[j]));
                    engine->AssimpManager.InsertTextures(MeshName, std::move(ptr), MeshIndex);

                    skip = true;
                    break;
                }
            }

            if (!skip)
            {
                Texture tex(Directory, str.C_Str(), type);
                tex.Load(false);
                textures.push_back(tex);
                Textures_loaded.push_back(tex);

                Graphics::textures.emplace(MeshName, tex);

                std::unique_ptr<Texture> ptr(new Texture(tex));
                engine->AssimpManager.InsertTextures(MeshName, std::move(ptr), MeshIndex);

            }
        }
        return textures;
    }
}