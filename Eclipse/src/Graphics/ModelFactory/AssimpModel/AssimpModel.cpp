#include "pch.h"
#include "AssimpModel/AssimpModel.h"

using namespace Eclipse;

AssimpModel::AssimpModel(glm::vec3 pos, glm::vec3 size, bool noTex)
    :
    pos(pos),
    size(size),
    noTex(noTex)
{

}

void AssimpModel::Render(Shader shader)
{
    shader.Use();

    auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetGameCameraID());
    GLint uniform_var_loc1 = shader.GetLocation("uModelToNDC");

    if (uniform_var_loc1 >= 0)
    {
        glm::mat4 mModelNDC = _camera.projMtx * _camera.viewMtx * glm::mat4(1.0f);
        glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mModelNDC));
    }

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        //auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");
        glBindFramebuffer(GL_FRAMEBUFFER, engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID());
        //shdrpgm->second.Use();

        meshes[i].Render(shader);
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
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

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