#include "pch.h"
#include "Graphics/ModelFactory/Sky/CubeMap.h"

Cubemap::Cubemap()
    : hasTextures(false) {}

void Cubemap::loadTextures(std::string _dir,
    std::string right,
    std::string left,
    std::string top,
    std::string bottom,
    std::string front,
    std::string back) {
    dir = _dir;
    hasTextures = true;
    faces = { right, left, top, bottom, front, back };

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    int width, height, nChannels;

    for (unsigned int i = 0; i < 6; i++) 
    {
        unsigned char* data = stbi_load((dir + "/" + faces[i]).c_str(),
            &width, &height, &nChannels, 0);

        GLenum colorMode = GL_RED;
        switch (nChannels) {
        case 3:
            colorMode = GL_RGB;
            break;
        case 4:
            colorMode = GL_RGBA;
            break;
        }

        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
        }
        else {
            std::cout << "Failed to load texture at " << faces[i] << std::endl;
        }

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::init() {
    // set up vertices
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &NEWVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(NEWVAO);

    // load data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GL_FLOAT)));
    glBindBuffer(GL_FLOAT, 0);

    glBindVertexArray(0);
}

void Cubemap::render(Shader& shader) 
{
    glBindVertexArray(NEWVAO);

    // Shader Activate
    shader.Use();

    glDepthMask(GL_FALSE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Check Main Uniforms
    auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetGameCameraID());
    GLint view = shader.GetLocation("view");
    GLuint projection = shader.GetLocation("projection");

    if (view >= 0)
    {
        glm::mat4 newview = glm::mat4(glm::mat3(_camera.viewMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(newview));
    }

    if (projection >= 0)
    {
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_camera.projMtx));
    }

    if (hasTextures) 
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }

    glBindVertexArray(NEWVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);

    shader.UnUse();
    glBindVertexArray(0);
}

void Cubemap::cleanup() {
    //VAO.cleanup();
}