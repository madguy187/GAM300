#include "pch.h"
#include "Mesh.h"

namespace Eclipse
{
    Eclipse::Mesh::Mesh(std::vector<Texture> textures) :
        Textures(textures),
        NoTex(false)
    {

    }

    Eclipse::Mesh::Mesh(aiColor4D diff, aiColor4D spec) :
        Diffuse(diff),
        Specular(spec),
        NoTex(true)
    {

    }

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string namein, std::vector<Texture> textures)
        :
        Vertices(vertices),
        Indices(indices),
        Textures(textures),
        NoTex(false),
        MeshName(namein)
    {
        Setup();
    }

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D specular, aiColor4D ambient, bool in, std::string namein)
        :
        Vertices(vertices),
        Indices(indices),
        Diffuse(diffuse),
        Specular(specular),
        Ambient(ambient),
        NoTex(in),
        MeshName(namein)
    {
        Setup();
    }

    void Mesh::Render(Shader& shader, GLenum mode, unsigned int id, unsigned int MeshIndex)
    {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, mode);

        // If dont have textures ( Flagged as True )
        if (NoTex && (!engine->world.CheckComponent<TextureComponent>(id)))
        {
            GLint uniform_var_loc1 = shader.GetLocation("BasicPrimitives");
            GLint uniform_var_loc2 = shader.GetLocation("uColor");
            GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
            GLuint tex_loc = shader.GetLocation("uTex2d");
            GLuint diff0 = shader.GetLocation("sdiffuse");
            GLuint spec = shader.GetLocation("sspecular");
            GLuint Texture = shader.GetLocation("noTex");

            glUniform1i(uniform_var_loc3, true);
            glUniform4f(diff0, Diffuse.r, Diffuse.g, Diffuse.b, Diffuse.a);
            glUniform4f(spec, Specular.r, Specular.g, Specular.b, Specular.a);
            glUniform1i(Texture, true);
            glUniform1i(tex_loc, false);
            glUniform1i(uniform_var_loc1, false);
        }
        else
        {
            if (engine->world.CheckComponent<TextureComponent>(id))
            {
                TextureComponent& tex = engine->world.GetComponent<TextureComponent>(id);

                // textures
                unsigned int diffuseIdx = 0;
                unsigned int specularIdx = 0;

                for (unsigned int i = 0; i < tex.HoldingTextures[MeshIndex].size(); i++)
                {
                    // activate texture
                    glActiveTexture(GL_TEXTURE0 + i);

                    // retrieve texture info
                    std::string name;
                    switch (tex.HoldingTextures[MeshIndex][i].GetType())
                    {
                    case aiTextureType_DIFFUSE:
                        name = "diffuse" + std::to_string(diffuseIdx++);
                        break;
                    case aiTextureType_SPECULAR:
                        name = "specular" + std::to_string(specularIdx++);
                        break;
                    }

                    GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
                    GLuint diff0 = shader.GetLocation("diffuse0");
                    GLuint spec = shader.GetLocation("specular0");
                    GLuint dsa = shader.GetLocation("noTex");

                    glUniform1i(uniform_var_loc3, true);
                    glUniform1i(diff0, i);
                    glUniform1i(spec, i);
                    glUniform1i(dsa, false);

                    // bind texture
                    tex.HoldingTextures[MeshIndex][i].Bind();
                }

                // If no Textures , We leave it blank until it has textures
                if (tex.HoldingTextures[MeshIndex].size() == 0)
                {
                    GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
                    glUniform1i(uniform_var_loc3, false);
                }
            }
            else
            {
                GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
                glUniform1i(uniform_var_loc3, false);
            }
        }

        // reset
        glActiveTexture(GL_TEXTURE0);

        // EBO stuff
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Mesh::Cleanup()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Mesh::GetAllTextures()
    {
        for (int i = 0; i < Textures.size(); i++)
        {
            std::cout << " Texture Name " << Textures[i].GetPath() << std::endl;
        }
    }

    std::vector<Vertex>& Mesh::GetVertices()
    {
        return Vertices;
    }

    unsigned int Mesh::GetVBOID()
    {
        return VBO;
    }

    void Mesh::Setup()
    {
        // Cr8 buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

        // set vertex attribute pointers
        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Textures C0ordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TextureCoodinates)));

        // Normsals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

        glBindVertexArray(0);
    }

    void Eclipse::Mesh::SetMaterialDiffuse(float x, float y, float z)
    {
        Diffuse.r = x;
        Diffuse.g = y;
        Diffuse.b = z;
    }

    void Eclipse::Mesh::SetMaterialSpecular(float x, float y, float z)
    {
        Specular.r = x;
        Specular.g = y;
        Specular.b = z;
    }

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
}