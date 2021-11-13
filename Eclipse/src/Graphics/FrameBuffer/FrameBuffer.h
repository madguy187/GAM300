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
      "MATERIALEDITORVIEW",
      "MESHEDITORVIEW",
      "GAMEVIEW_SOBEL",
      "SHADOWVIEW",
      "MAXCOUNT"
    };

    class FrameBuffer
    {

    public:
        glm::uvec2 m_size{ 0,0 };
        unsigned int m_width = 0;
        unsigned int m_height = 0;
        FrameBufferMode FrameBufferType = FrameBufferMode::FBM_NONE;
        float AspectRatio = 1.0f;

        struct FramebufferData
        {
            unsigned int frameBufferID = 0;
            unsigned int depthBufferID = 0;
            //unsigned int TextureColourBuffer = 0;
            unsigned int ColorBuffers[2];
            FrameBufferMode hiddentype = FrameBufferMode::FBM_NONE;
        };

        FramebufferData m_data;
        RenderMode m_renderMode = RenderMode::Fill_Mode;
        ECVec2 windowPos{ 0,0 };
        ECVec2 frameBufferPosition{ 0,0 };
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
        void Resize(unsigned width, unsigned height, FrameBufferMode Mode);
        static void ShowWindow(FrameBuffer g);
        void CreateFrameBuffer(unsigned int p_width, unsigned int p_height);
        void FrameBuffer::DeleteBuffers();
        void CreateShadowFBO(unsigned int p_width, unsigned int p_height);
        void FrameBuffer::SetViewPort() const;

    public:
        // POST PROCESS HERE ==========================
        enum class PostProcessType : unsigned int
        {
            PPT_NONE = 0,
            PPT_INVERSE = 1,
            PPT_GREY = 2,
            PPT_KERNEL = 3,
            PPT_BLUR = 4,
            PPT_SOBEL = 5,
            PPT_BLLEDING = 6,
            PPT_SCREENSHAKE = 7,
            PPT_MAXCOUNT
        };
        PostProcessType PPType_ = PostProcessType::PPT_NONE;
        bool AllowPostProcess = false;
        unsigned int rectVAO = 0;
        unsigned int rectVBO = 0;
        void CreatePostProcessFramebuffer();

        // Sobel
        float FadeInTimer = 0.0f;
        float Multiplier = 30.0f;
        float Visible = 1.0f;

        // Bleeding
        float BleedingTimer = 0.0f;
    };
}
#endif//FRAMEBUFFER_H