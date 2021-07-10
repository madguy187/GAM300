#include "pch.h"
#include "AssimpModel/AssimpModel.h"
#include <regex>

using namespace Eclipse;

AssimpModel::AssimpModel(bool noTex)
    :
    noTex(noTex)
{
    GlobalMode = GL_FILL;
}

void AssimpModel::Render(Shader& shader, GLenum MOde ,unsigned int FrameBufferID)
{
    // Check Main Uniforms
    auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
    CheckUniformLoc(shader, _camera, FrameBufferID);

    for (unsigned int i = 0; i < Meshes.size(); i++)
    {
        Meshes[i].Render(shader, MOde);
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
    unsigned int importOptions = aiProcess_Triangulate
        | aiProcess_OptimizeMeshes
        | aiProcess_JoinIdenticalVertices
        | aiProcess_Triangulate
        | aiProcess_CalcTangentSpace
        | aiProcess_FlipUVs;

    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, importOptions);

    //if (scene->GetEmbeddedTexture("src/Assets/ASSModels/dog/1.FBX") == nullptr)
    //{
    //    std::cout << "fuck" << std::endl;
    //}

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::string Error = ("Could not load AssimpModel at " + path + import.GetErrorString()).c_str();
        ENGINE_LOG_ASSERT(false, Error);
        return;
    }

    Directory = path.substr(0, path.find_last_of("/"));
    ProcessNode(scene->mRootNode, scene);
}

void AssimpModel::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Meshes.push_back(ProcessMesh(mesh, scene));
    }

    // process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

void AssimpModel::CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int FrameBufferID)
{
    TransformComponent& Transform = engine->world.GetComponent<TransformComponent>(ID);

    GLint uModelToNDC_ = _shdrpgm.GetLocation("uModelToNDC");
    GLuint model_ = _shdrpgm.GetLocation("model");

    if (uModelToNDC_ >= 0)
    {
        glm::mat4 mModelNDC;

        // Everything Below this Comment is To be Removed !!
        if (NameOfModel == "Black Dog")
        {
            Transform.position.setX(20);
        }
        else if (NameOfModel == "White Dog")
        {
            Transform.position.setX(-20);
        }
        else
        {
            Transform.position.setX(0);
        }
        ///////////////////////////////////////////////////////

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, Transform.position.ConvertToGlmVec3Type());
        model = glm::rotate(model, glm::radians(Transform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Transform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Transform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, Transform.scale.ConvertToGlmVec3Type());

        mModelNDC = _camera.projMtx * _camera.viewMtx * model;
        glUniformMatrix4fv(uModelToNDC_, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
    }
}

void AssimpModel::SetName(std::string name)
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

Mesh AssimpModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

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

        vertices.push_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
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
            //regular file, check if it exists and read it
            if (noTex)
            {
                // diffuse color
                aiColor4D diff(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);

                // specular color
                aiColor4D spec(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

                return Mesh(vertices, indices, diff, spec);
            }

            // diffuse maps
            std::vector<Texture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // specular maps
            std::vector<Texture> specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> AssimpModel::LoadTextures(aiMaterial* mat, aiTextureType type)
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
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            // not loaded yet
            //std::cout << str.C_Str() << std::endl;

            Texture tex(Directory, str.C_Str(), type);
            tex.Load(false);
            textures.push_back(tex);
            Textures_loaded.push_back(tex);
        }
    }

    return textures;
}
