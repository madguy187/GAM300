#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "Parser.h"

namespace Eclipse
{
  class OpenGL_Context
  {
  public:
    void LoadConfigData(std::string configFile);
    bool init(std::string configFile);
    void print_specs();
    void pre_render();
    void post_render();
    void end();

    static GLint width, height;
    GLint prevWidth, prevHeight;
    float windowRatioX, windowRatioY;
    GLdouble fps;
    std::string title;
    static GLFWwindow* ptr_window;
    static GLdouble m_posX, m_posY;
    void on_key(int key, int scancode, int action, int mods);
    void on_resize(int width, int height);
    static void on_close();
    static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);
    static void fbsize_cb(GLFWwindow* ptr_win, int _width, int _height);
    static void key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
    static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
  };
}
