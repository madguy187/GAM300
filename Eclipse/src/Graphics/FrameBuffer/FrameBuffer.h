#ifndef VIEW_H
#define VIEW_H

namespace Eclipse
{
    enum class FrameBufferMode
    {
        NONE = 0,
        GAMEVIEW = 1,
        SCENEVIEW = 2,
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
        void Resize(unsigned width, unsigned height);

        static void ShowWindow(FrameBuffer g, const char* input);

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
        ECVec2 windowPos;
        ECVec2 frameBufferPosition;
        glm::uvec2 m_size;
        unsigned int m_width, m_height;

        friend std::ostream& operator << (std::ostream& os, const FrameBufferMode& in);
        void CreateFrameBuffer(unsigned int p_width, unsigned int p_height);

    };
}
#endif//VIEW_H