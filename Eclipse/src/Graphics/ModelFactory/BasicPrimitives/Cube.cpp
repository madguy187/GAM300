#include "pch.h"
#include "Cube.h"

Cube::Cube() :
    vaoID{ 1 }, vboID{ 1 }, eboID{ 1 },
    primitiveType{ GL_TRIANGLES },
    primitiveCount{ 2 },
    drawCount{ 6 },
    TriCount{ 0 },
    pos(glm::vec3(0.0f)),
    size(glm::vec3(1.0f)),
    noTex(false)
{
    initModel();
}

void Cube::initModel()
{
    LoadModel("src/Assets/ASSModels/lotr_troll/scene.gltf");
    //CreateBuffers();

}

void Cube::InsertPosVtx()
{
    Plane_ planemesh;

    glm::vec3 const translateArray[] =
    {
        glm::vec3(0.0f,  0.0f,  0.5f), // Z+
        glm::vec3(0.0f,  0.0f, -0.5f), // Z-
        glm::vec3(0.5f,  0.0f,  0.0f), // X+
        glm::vec3(-0.5f,  0.0f,  0.0f), // X-
        glm::vec3(0.0f,  0.5f,  0.0f), // Y+
        glm::vec3(0.0f, -0.5f,  0.0f), // Y-
    };

    glm::vec2 const rotateArray[] =
    {
        glm::vec2(0.0f, 0.0f),             // Z+
        glm::vec2(0.0f, (float)PI),        // Z-
        glm::vec2(0.0f, (float)HALF_PI),   // X+       
        glm::vec2(0.0f, (float)HALF_PI),   // X-
        glm::vec2((float)HALF_PI, 0.0f),   // Y+
        glm::vec2((float)HALF_PI, 0.0f)    // Y-
    };

    /*  Transform the plane to 6 positions to form the faces of the cube */
    for (int i = 0; i < 6; ++i)
    {
        Mat4 transformMat = Translate(translateArray[i]) * Rotate(rotateArray[i][YINDEX], YAXIS) * Rotate(rotateArray[i][XINDEX], XAXIS);

        for (int j = 0; j < 4; ++j)
        {
            glm::vec3 posVtx = glm::vec3(transformMat * glm::vec4(planemesh.PosVec[j], 1.0));
            glm::vec3 nrmVtx = glm::vec3(transformMat * glm::vec4(planemesh.NormalVec[j], 1.0));

            posVtx = RoundDecimal(posVtx);
            nrmVtx = RoundDecimal(nrmVtx);

            PosVec.push_back(posVtx);
            NormalVec.push_back(nrmVtx);
            TextVec.push_back(planemesh.TextVec[j]);
        }
    }
}

void Cube::InsertIdxVtx()
{
    Plane_ planemesh;

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            IdxVec.push_back(planemesh.IdxVec[j] + 4 * i);
        }
    }
}

void Cube::InsertTextCoord()
{
}

void Cube::InsertModelData()
{
    InsertPosVtx();
    InsertIdxVtx();
}

GLuint Cube::GetVaoID()
{
    return vaoID;
}

GLuint Cube::GetVboID()
{
    return vboID;
}

GLuint Cube::GetEboID()
{
    return eboID;
}

GLenum Cube::GetPrimitiveType()
{
    return primitiveType;
}

GLuint Cube::GetPrimitiveCount()
{
    return primitiveCount;
}

GLuint Cube::GetDrawCount()
{
    //return drawCount;
    return static_cast<GLuint>(IdxVec.size());
}

void Cube::SetVaoID(GLuint id)
{
    this->vaoID = id;
}

void Cube::SetVboID(GLuint id)
{
    this->vboID = id;
}

void Cube::SetEboID(GLuint id)
{
    this->eboID = id;
}

void Cube::SetPrimitiveType(GLenum type)
{
    this->primitiveType = type;
}

void Cube::SetPrimitiveCount(GLuint count)
{
    this->primitiveCount = count;
}

void Cube::SetDrawCount(GLuint count)
{
    this->drawCount = count;
}

void Cube::CreateVAO()
{
    //Define the VAO handle for position attributes
    glCreateVertexArrays(1, &vaoID);
    glEnableVertexArrayAttrib(vaoID, 0);
    glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, sizeof(glm::vec3));
    glVertexArrayAttribFormat(vaoID, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoID, 0, 0);

    //Define the VAO handle for texture coordinates
    glEnableVertexArrayAttrib(vaoID, 1);
    glVertexArrayVertexBuffer(vaoID, 1, vboID, sizeof(glm::vec3) * PosVec.size(), sizeof(glm::vec2));
    glVertexArrayAttribFormat(vaoID, 1, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoID, 1, 1);

}

void Cube::CreateVBO()
{
    glCreateBuffers(1, &vboID);
    glNamedBufferStorage(vboID,
        sizeof(glm::vec3) * PosVec.size() + sizeof(glm::vec2) * TextVec.size(),
        nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(vboID, 0,
        sizeof(glm::vec3) * PosVec.size(), PosVec.data());
    glNamedBufferSubData(vboID, sizeof(glm::vec3) * PosVec.size(),
        sizeof(glm::vec2) * TextVec.size(), TextVec.data());
}

void Cube::CreateEBO()
{
    glCreateBuffers(1, &eboID);
    glNamedBufferStorage(eboID, sizeof(GLushort) * IdxVec.size(), reinterpret_cast<GLvoid*>(IdxVec.data()), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vaoID, eboID);
    glBindVertexArray(0);
}

void Cube::CreateBuffers()
{
    CreateVBO();
    CreateVAO();
    CreateEBO();
}

void Cube::DeleteModel()
{
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &eboID);
}

bool Cube::DegenerateTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    return (glm::distance(v0, v1) < EPSILON ||
        glm::distance(v1, v2) < EPSILON ||
        glm::distance(v2, v0) < EPSILON);
}

GLuint Cube::GetIndicesCount()
{
    return static_cast<GLuint>(IdxVec.size());
}

void Cube::LoadModel(std::string pathname)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(pathname, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Could not Load model at " << pathname << std::endl << import.GetErrorString() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    directory = pathname.substr(0, pathname.find_last_of("/"));
    processNode(scene->mRootNode, scene);
}

void Cube::processNode(aiNode* node, const aiScene* scene)
{
    // process all meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

std::vector<Texture> Cube::loadTextures(aiMaterial* mat, aiTextureType type)
{
    std::vector <Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::cout << str.C_Str() << std::endl;

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

Mesh Cube::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // position
        vertex.pos = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        // normal vectors
        vertex.normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        // textures
        if (mesh->mTextureCoords[0]) {
            vertex.texCoord = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else {
            vertex.texCoord = glm::vec2(0.0f);
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
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        std::cout << " Test : " << noTex << std::endl;

        if (noTex) {
            // diffuse color
            aiColor4D diff(1.0f);
            aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);

            // specular color
            aiColor4D spec(1.0f);
            aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

            return Mesh(vertices, indices, diff, spec);
        }

        // diffuse maps
        std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // specular maps
        std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

void Cube::render(Shader shader)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, pos);
    model = glm::scale(model, size);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setMat4("model", model);

    shader.setFloat("material.shininess", 0.5f);

    for (unsigned int i = 0; i < meshes.size(); i++) {

        meshes[i].render(shader);
    }
}