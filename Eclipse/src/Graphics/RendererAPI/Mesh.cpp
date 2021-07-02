#include "pch.h"
#include "Mesh.h"

using namespace Eclipse;

std::vector<Vertex> Vertex::genList(float* vertices, int noVertices) {
    std::vector<Vertex> ret(noVertices);

    int stride = sizeof(Vertex) / sizeof(float);

    for (int i = 0; i < noVertices; i++) {
        ret[i].pos = glm::vec3(
            vertices[i * stride + 0],
            vertices[i * stride + 1],
            vertices[i * stride + 2]
        );

        ret[i].normal = glm::vec3(
            vertices[i * stride + 3],
            vertices[i * stride + 4],
            vertices[i * stride + 5]
        );

        ret[i].texCoord = glm::vec2(
            vertices[i * stride + 6],
            vertices[i * stride + 7]
        );
    }

    return ret;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    : vertices(vertices), indices(indices), textures(textures), noTex(false) {
    setup();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D specular)
    : vertices(vertices), indices(indices), diffuse(diffuse), specular(specular), noTex(true) {
    setup();
}

void Mesh::render(Shader shader) {

    shader.Use();

    glBindVertexArray(VAO);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (noTex) 
    {
        // materials
        auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");

        GLint uniform_var_loc0 = shdrpgm->second.GetLocation("material.diffuse");
        GLint uniform_var_loc1 = shdrpgm->second.GetLocation("material.specular");
        GLint uniform_var_loc2 = shdrpgm->second.GetLocation("noTex");

        if (uniform_var_loc0 >= 0)
        {
            GLCall(glUniform4f(uniform_var_loc0, diffuse.r, diffuse.g, diffuse.b, diffuse.a));
        }

        if (uniform_var_loc1 >= 0)
        {
            GLCall(glUniform4f(uniform_var_loc0, specular.r, specular.g, specular.b, specular.a));
        }

        if (uniform_var_loc2 >= 0)
        {
            GLCall(glUniform1i(uniform_var_loc2, 1));
        }
    }
    else {
        // textures
        unsigned int diffuseIdx = 0;
        unsigned int specularIdx = 0;

        for (unsigned int i = 0; i < textures.size(); i++) {
            // activate texture
            glActiveTexture(GL_TEXTURE0 + i);

            // retrieve texture info
            std::string name;
            switch (textures[i].type) {
            case aiTextureType_DIFFUSE:
                name = "diffuse" + std::to_string(diffuseIdx++);
                break;
            case aiTextureType_SPECULAR:
                name = "specular" + std::to_string(specularIdx++);
                break;
            }

            //std::cout << name << std::endl;

            GLint uniform_var_loc2 = shader.GetLocation("uColor");
            GLint uniform_var_loc3 = shader.GetLocation("uTextureCheck");
            GLuint tex_loc = shader.GetLocation("uTex2d");

            if (uniform_var_loc2 >= 0)
            {
                glUniform4f(uniform_var_loc2, 0.5, 0,0,1);
            }

            if (uniform_var_loc3 >= 0)
            {
                glUniform1i(uniform_var_loc3, true);
            }

            if (tex_loc >= 0)
            {
                glUniform1i(tex_loc, i);
            }

            // set the shader value
            //shader.setInt(name, i);

            // bind texture
            textures[i].bind();
        }
    }

    // EBO stuff
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // reset
    glActiveTexture(GL_TEXTURE0);

    shader.UnUse();
}

void Mesh::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::setup() {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO); // vertex array object
    glGenBuffers(1, &VBO); // vertex buffer object
    glGenBuffers(1, &EBO); // element buffer object

    glBindVertexArray(VAO);

    // load data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // load data into EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set vertex attribute pointers
    // vertex.position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex.normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    // vertex.texCoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));

    glBindVertexArray(0);
}