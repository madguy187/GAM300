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

void AssimpModel::Render(Shader& shader, GLenum MOde)
{
    // Shader Activate
    shader.Use();

    // Check Main Uniforms
    auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
    CheckUniformLoc(shader, _camera);

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Render(shader, MOde);
    }

    shader.UnUse();
}

void AssimpModel::Cleanup()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Cleanup();
    }
}

void AssimpModel::LoadAssimpModel(std::string path)
{

    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

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

    directory = path.substr(0, path.find_last_of("/"));
    ProcessNode(scene->mRootNode, scene);
}

void AssimpModel::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    // process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

void AssimpModel::CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera)
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
            Transform.position.setX(10);
        }
        else
        {
            Transform.position.setX(-10);
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
    return directory;
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
        std::regex regexExpress("^\\*\\d+");
        std::cmatch regexMatches;

        //for (size_t j = 0; j < material->GetTextureCount(aiTextureType_DIFFUSE); j++)
        //{      
        //    // The path to the texture.
        //    aiString texturePath;

        //    // Get the path to the texture.
        //    if (material->GetTexture(aiTextureType_LIGHTMAP, j, &texturePath) ==
        //        aiReturn_SUCCESS)
        //    {
        //        // Check if it's an embedded or external  texture.
        //        if (std::regex_search(texturePath.C_Str(), regexMatches, regexExpress))
        //        {
        //            // Get the index str.
        //            std::string indexStr = *(regexMatches.begin());

        //            // Drop the "*" character.
        //            indexStr = indexStr.erase(0, 1);

        //            // Convert the string to an integer. (This is the index in the
        //            // Scene::mTextures[] array.
        //            int index = std::stoi(indexStr);

        //            // Print the index.
        //            std::cout << "Scene::mTextures[" << index << "]." << std::endl;
        //        }
        //        else
        //        {
        //            // Print the texture file path.
        //            std::cout << "File Path: " << texturePath.C_Str() << std::endl;
        //        }
        //    }
        //}

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

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> AssimpModel::LoadTextures(aiMaterial* mat, aiTextureType type)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        //std::cout << str.C_Str() << std::endl;

        // prevent duplicate loading
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        //std::cout << "TEST" << std::endl;

        if (!skip)
        {
            // not loaded yet
            Texture tex(directory, str.C_Str(), type);
            tex.load(false);
            textures.push_back(tex);
            textures_loaded.push_back(tex);
        }
    }

    return textures;
}

void AssimpModel::FBXLoadAssimpModel(const char* in)
{
    const aiScene* scene = aiImportFile(in, aiProcessPreset_TargetRealtime_MaxQuality);

    if (!scene)
    {
        std::cout << "Could not load file " << in << std::endl;
    }

    std::vector<Mesh> meshes;
    meshes.reserve(scene->mNumMeshes);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
    {
        aiMesh* mesh = scene->mMeshes[meshIdx];

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor4D specularColor;
        aiColor4D diffuseColor;
        aiColor4D ambientColor;
        float shininess;

        aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specularColor);
        aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
        aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
        aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);

        for (int vertIdx = 0; vertIdx < mesh->mNumVertices; vertIdx++)
        {
            Vertex vertex;
            aiVector3D vert = mesh->mVertices[vertIdx];
            aiVector3D norm = mesh->mNormals[vertIdx];

            // position
            vertex.Position = glm::vec3(vert.x, vert.y, vert.z);

            // normal vectors
            vertex.Normal = glm::vec3(norm.x, norm.y, norm.z);

            vertices.push_back(vertex);
        }

        std::vector<unsigned int> indices;
        indices.reserve(mesh->mNumFaces * 3);

        for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++)
        {
            indices.push_back(mesh->mFaces[faceIdx].mIndices[0u]);
            indices.push_back(mesh->mFaces[faceIdx].mIndices[1u]);
            indices.push_back(mesh->mFaces[faceIdx].mIndices[2u]);
        }

        Mesh Test = Mesh(vertices, indices, textures);
    }
}