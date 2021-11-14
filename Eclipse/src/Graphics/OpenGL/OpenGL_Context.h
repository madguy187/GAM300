#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "Parser.h"

namespace Eclipse
{
    enum class BlendMode
    {
        INTERPOLATE,
        PREMULTIPLY,
        NOBLEND,
        ADDITIVE,
        MULTIPLY,
        LIGHTMAP,
        MAXCOUNT
    };

    class OpenGL_Context
    {
    public:
        inline static GLint width, height;
        GLint prevWidth, prevHeight;
        inline static float windowRatioX, windowRatioY;
        inline static std::string title;
        inline static GLFWwindow* ptr_window;
        inline static GLdouble m_posX, m_posY;
        inline static GLdouble winPosX, winPosY;
    public:

        void LoadConfigData(std::string configFile);
        bool init(std::string configFile);
        void print_specs();
        void pre_render();
        static void post_render();
        void end();
        void CreateFrameBuffers();
        void SetBlendMode(BlendMode p_mode);
        void SetClearColor(const glm::vec4& p_color);
        void SetViewport(unsigned int p_x, unsigned int p_y, unsigned int p_width, unsigned int p_height);
        static void CreateFrameBuffers(unsigned int width, unsigned int height, FrameBufferMode);

        static GLFWwindow* GetWindow();
        static GLint GetWidth();
        static GLint GetHeight();
        static float GetWindowRatioX();
        static float GetWindowRatioY();
        static glm::vec2 GetMouseCursorPos();
        static glm::vec2 GetContextPosition();
        FrameBuffer* GetFramebuffer(FrameBufferMode mode);

        void on_key(int key, int scancode, int action, int mods);
        void on_resize(int width, int height);
        static void on_close();
        static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);
        static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
        static void window_pos_callback(GLFWwindow* pwin, int xpos, int ypos);

        //Remove when delta-time implementation is added - Rachel
        static double update_time(double fps_calc_interval);
        static GLdouble deltaTime;
        static GLdouble fps;
    private:
        void Init();
        void Clear();      
    };
}