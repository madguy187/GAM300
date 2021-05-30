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

    // Clear the view
    void pre_render();

    // Render end, swap buffers
    void post_render();
    void end();

    static GLint width, height;
    GLint prevWidth, prevHeight;
    static float windowRatioX, windowRatioY;
    static GLdouble fps;
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


    //Remove when delta-time implementation is added - Rachel
    static double update_time(double fps_calc_interval);
    static GLdouble deltaTime;
  };
}
