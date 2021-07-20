#include "pch.h"
#include "Graphics/ModelFactory/Sky/CubeMap.h"

namespace Eclipse
{
    CubeMap::CubeMap() :
        HasTextures(false),
        VAO(1),
        VBO(1),
        Id(1),
        EBO(1)
    {
    }

    void CubeMap::LoadTextures(std::string _dir,
        std::string right, std::string left,
        std::string top, std::string bottom,
        std::string front, std::string back)
    {
        Load(_dir, right, left, top, bottom, front, back);

        glGenTextures(1, &Id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, Id);

        int Width, Height, Channels;
        Setup(Height, Width, Channels);

        std::string Msg = ("Cube Map at [" + _dir + "] is loaded Successfully").c_str();
        ENGINE_CORE_INFO(Msg);
    }

    void CubeMap::CreateSky(std::string _dir)
    {
        LoadTextures(_dir);

        // Set up the vertixes
        float skyboxVertices[] =
        {
            // Positions        
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

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        // load data into VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), skyboxVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GL_FLOAT)));
        glBindBuffer(GL_FLOAT, 0);

        glBindVertexArray(0);

        EDITOR_LOG_INFO("CubeMap : Environment Created");
    }

    void CubeMap::Render(Shader& shader)
    {
        if (Visible == false)
            return;

        glBindVertexArray(VAO);

        // Shader Activate
        shader.Use();

        glDepthMask(GL_FALSE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Check Main Uniforms
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetGameCameraID());
        GLint view = shader.GetLocation("view");
        GLuint projection = shader.GetLocation("projection");

        glm::mat4 _cameraprojMtx = glm::perspective(glm::radians(_camera.fov), _camera.aspect, 0.1f, _camera.farPlane);

        glm::mat4 newview = glm::mat4(glm::mat3(_camera.viewMtx));
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(newview));
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_cameraprojMtx));

        if (HasTextures)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, Id);
        }

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);

        shader.UnUse();
        glBindVertexArray(0);
    }

    void CubeMap::Cleanup()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    std::string Eclipse::CubeMap::GetDirectory()
    {
        return Directory;
    }

    void Eclipse::CubeMap::SetDirectory(std::string in)
    {
        Directory = in;
    }

    unsigned int Eclipse::CubeMap::GetVaoID()
    {
        return VAO;
    }

    unsigned int Eclipse::CubeMap::GetVboID()
    {
        return VBO;
    }

    unsigned int Eclipse::CubeMap::GetEboID()
    {
        return EBO;
    }

    void Eclipse::CubeMap::SetTextureFlag(bool in)
    {
        HasTextures = in;
    }

    bool Eclipse::CubeMap::GetTextureFlag()
    {
        return HasTextures;
    }

    unsigned int Eclipse::CubeMap::GetFaceCount()
    {
        return Faces.size();
    }

    void Eclipse::CubeMap::SetVisible(bool in)
    {
        Visible = in;
    }

    bool Eclipse::CubeMap::GetVisibllity()
    {
        return Visible;
    }

    void Eclipse::CubeMap::Setup(int& height_, int& width_, int& channel)
    {
        for (unsigned int i = 0; i < 6; i++)
        {
            unsigned char* data = stbi_load((Directory + "/" + Faces[i]).c_str(), &width_, &height_, &channel, 0);

            GLenum colorMode = GL_RED;

            switch (channel)
            {
            case 3:
                colorMode = GL_RGB;
                break;
            case 4:
                colorMode = GL_RGBA;
                break;
            }

            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, colorMode, width_, height_, 0, colorMode, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                std::cout << "Failed to load texture at " << Faces[i] << std::endl;
            }

            stbi_image_free(data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void Eclipse::CubeMap::Load(std::string _dir, std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back)
    {
        Directory = _dir;
        HasTextures = true;
        Faces.push_back(right);
        Faces.push_back(left);
        Faces.push_back(top);
        Faces.push_back(bottom);
        Faces.push_back(front);
        Faces.push_back(back);
    }
}