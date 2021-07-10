#include "pch.h"
#include "Mesh.h"

using namespace Eclipse;

std::vector<Vertex> Vertex::GenList(float* vertices, int noVertices)
{
    std::vector<Vertex> ret(noVertices);

    int stride = sizeof(Vertex) / sizeof(float);

    for (int i = 0; i < noVertices; i++)
    {
        ret[i].Position = glm::vec3(vertices[i * stride + 0], vertices[i * stride + 1], vertices[i * stride + 2]);
        ret[i].Normal = glm::vec3(vertices[i * stride + 3], vertices[i * stride + 4], vertices[i * stride + 5]);
        ret[i].TextureCoodinates = glm::vec2(vertices[i * stride + 6], vertices[i * stride + 7]);
    }

    return ret;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    :
    Vertices(vertices),
    Indices(indices),
    Textures(textures),
    NoTex(false)
{
    Setup();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D specular)
    :
    Vertices(vertices),
    Indices(indices),
    Diffuse(diffuse),
    Specular(specular),
    NoTex(true)
{
    Setup();
}

void Mesh::Render(Shader& shader, GLenum mode)
{
    // glBindVertexArray(VAO);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, mode);

    // textures
    unsigned int diffuseIdx = 0;
    unsigned int specularIdx = 0;

    for (unsigned int i = 0; i < Textures.size(); i++)
    {
        // activate texture
        glActiveTexture(GL_TEXTURE0 + i);

        // retrieve texture info
        std::string name;
        switch (Textures[i].GetType())
        {
        case aiTextureType_DIFFUSE:
            name = "diffuse" + std::to_string(diffuseIdx++);
            break;
        case aiTextureType_SPECULAR:
            name = "specular" + std::to_string(specularIdx++);
            break;
        }

        GLint uniform_var_loc2 = shader.GetLocation("uColor");
        GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
        GLuint tex_loc = shader.GetLocation("uTex2d");

        if (uniform_var_loc2 >= 0)
        {
            glUniform4f(uniform_var_loc2, 0.5, 0, 0, 1);
        }

        if (uniform_var_loc3 >= 0)
        {
            glUniform1i(uniform_var_loc3, true);
        }

        if (tex_loc >= 0)
        {
            glUniform1i(tex_loc, i);
        }
        // bind texture
        //Textures[i].bind();
        glBindTexture(GL_TEXTURE_2D, Textures[i].GetId());
    }
    // EBO stuff
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // reset
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::Cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::Setup()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // load data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

    // load data into EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

    // set vertex attribute pointers
    // vertex.position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex.texCoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TextureCoodinates)));

    // vertex.normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

    glBindVertexArray(0);
}