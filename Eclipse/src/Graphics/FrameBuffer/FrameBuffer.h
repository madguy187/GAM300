#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

namespace Eclipse
{
    static const char* enum_FrameBufferMode_str[] =
    {
      "NONE",
      "GAMEVIEW",
      "SCENEVIEW",
      "SWITCHINGVIEWS_TOP",
      "SWITCHINGVIEWS_BOTTOM",
      "SWITCHINGVIEWS_LEFT",
      "SWITCHINGVIEWS_RIGHT",
      "MAXCOUNT"
    };

    class FrameBuffer
    {

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
        RenderMode m_renderMode = RenderMode::Fill_Mode;
        ECVec2 windowPos;
        ECVec2 frameBufferPosition;
        glm::uvec2 m_size;
        unsigned int m_width, m_height;
        std::string Name;

        friend std::ostream& operator << (std::ostream& os, const FrameBufferMode& in);
        std::string getStringForEnum(int enum_val);

    public:
        FrameBuffer() {};
        FrameBuffer(const glm::uvec2& p_size, FrameBufferMode in);
        FrameBuffer(unsigned int p_width, unsigned int p_height, FrameBufferMode in);
        ~FrameBuffer();
        void SetRenderMode(RenderMode _renderMode);
        RenderMode GetRenderMode();
        void DeletCurrentFrameBuffer();
        unsigned int GetFrameBufferID();
        unsigned int GetTextureColourBufferID();
        unsigned int GetDepthBufferID();
        FrameBufferMode GetFrameBufferType();
        std::string GetName();
        void Bind() const;
        void Unbind() const;
        void Clear() const;
        void Resize(unsigned width, unsigned height);
        static void ShowWindow(FrameBuffer g, const char* input);
        void CreateFrameBuffer(unsigned int p_width, unsigned int p_height);


    public:
        // POST PROCESS HERE ==========================
        enum class PostProcessType : unsigned int
        {
            PPT_NONE = 0,
            PPT_INVERSE = 1,
            PPT_GREY = 2,
            PPT_KERNEL = 3,
            PPT_MAXCOUNT
        };

        PostProcessType PPType_ = PostProcessType::PPT_NONE;
        bool AllowPostProcess = false;
        unsigned int rectVAO = 0;
        unsigned int rectVBO = 0;
        void CreatePostProcessFramebuffer();
        void UpdatePP();
    };
}
#endif//FRAMEBUFFER_H