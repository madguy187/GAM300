#ifndef VIEW_H
#define VIEW_H
#include "Vec.h"

namespace Eclipse
{
    enum class FrameBufferMode
    {
        None,
        GAMEVIEW,
        SCENEVIEW,
        MAXCOUNT
    };

    class FrameBuffer
    {
    public:
        FrameBuffer(const glm::uvec2& p_size, FrameBufferMode in);
        FrameBuffer(unsigned int p_width, unsigned int p_height, FrameBufferMode in);
        ~FrameBuffer();

        void Bind() const;
        void Unbind() const;
        void Clear() const;

        void Init();
        void Update();
        static void ShowWindow(FrameBuffer g, const char* input);
        void CreateFrameBuffer(unsigned int p_width, unsigned int p_height);

        unsigned int GetFrameBufferID();
        unsigned int GetTextureColourBufferID();
        unsigned int GetDepthBufferID();
        FrameBufferMode GetFrameBufferType();

    private:

        struct FramebufferData
        {
            unsigned int frameBufferID;
            unsigned int depthBufferID;
            unsigned int TextureColourBuffer;
            FrameBufferMode hiddentype;
        };

        FrameBufferMode FrameBufferType;
        FramebufferData m_data;
        vec2 windowPos;
        vec2 frameBufferPosition;
        glm::uvec2 m_size;
        unsigned int m_width, m_height;

    };
}
#endif//VIEW_H