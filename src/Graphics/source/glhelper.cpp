/**********************************************************************************
* \file            GLHelper.cpp
*
* \brief        Definition of the GLHelper class function(s).
*
* \author       Lin Yanping Rachel
*
* \email		l.yanpingrachel@digipen.edu
*
* \date			10 oct 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
/*!
This file implements functionality useful and necessary to build OpenGL
applications including use of external APIs such as GLFW to create a
window and start up an OpenGL context and use GLEW to extract function
pointers to OpenGL implementations.

*//*__________________________________________________________________________*/
#include "pch.h"
#include "../src/Graphics/include/GLHelper.h"

static GLHelper g_glhelper;

// static data members declared in GLHelper
GLint GLHelper::width;
GLint GLHelper::height;
GLint GLHelper::prevWidth;
GLint GLHelper::prevHeight;
float GLHelper::windowRatioX;
float GLHelper::windowRatioY;
GLdouble GLHelper::fps;
GLdouble GLHelper::m_posX;
GLdouble GLHelper::m_posY;
GLdouble GLHelper::deltaTime;
std::string GLHelper::title;
GLFWwindow* GLHelper::ptr_window;

/*  _________________________________________________________________________ */
/*! init

@param GLint width
@param GLint height
Dimensions of window requested by program

@param std::string title_str
String printed to window's title bar

@return bool
true if OpenGL context and GLEW were successfully initialized.
false otherwise.

@amendments
Added command to specify use of OpenGL that is forward compatible.
Added command to specify use of a 24-bit depth buffer.

Uses GLFW to create OpenGL context. GLFW's initialization follows from here:
http://www.glfw.org/docs/latest/quick.html
a window of size width x height pixels
and its associated OpenGL context that matches a core profile that is
compatible with OpenGL 4.5 and doesn't support "old" OpenGL, has 32-bit RGBA,
double-buffered color buffer, 24-bit depth buffer and 8-bit stencil buffer
with each buffer of size width x height pixels
*/

void GLHelper::LoadConfigData(std::string configFile)
{
  Parser input;
  input.ParseFile(configFile);

  title = input.doc["title"].GetString();
  width = input.doc["dimensions"][rapidjson::SizeType(0)].GetInt();
  height = input.doc["dimensions"][rapidjson::SizeType(1)].GetInt();
}

bool GLHelper::init(std::string configFile)
{
    LoadConfigData(configFile);

    // Part 1
    if (!glfwInit())
    {
        std::cout << "GLFW init has failed - abort program!!!" << std::endl;
        return false;
    }

    // In case a GLFW function fails, an error is reported to callback function
    glfwSetErrorCallback(GLHelper::error_cb);

    // Before asking GLFW to create an OpenGL context, we specify the minimum constraints
    // in that context:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //Added to specify use of OpenGL that is forward compatible
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    //Added to specify use of a 24-bit depth buffer
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

    GLHelper::prevWidth = GLHelper::width;
    GLHelper::prevHeight = GLHelper::height;

    /*Code for to enable full-screen mode */
    //const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    //int newWidth, newHeight;
    //newWidth = mode->width;
    //newHeight = mode->height;
    //GLHelper::width = newWidth;
    //GLHelper::height = newHeight;
    //SpawnPositions.FullScreen = true;
    //GLHelper::ptr_window = glfwCreateWindow(newWidth, newHeight, title.c_str(), glfwGetPrimaryMonitor(), NULL);

    GLHelper::ptr_window = glfwCreateWindow(width, height, title.c_str(), NULL(), NULL);
    GLHelper::windowRatioX = static_cast<float>(GLHelper::width) / static_cast<float>(GLHelper::prevWidth);
    GLHelper::windowRatioY = static_cast<float>(GLHelper::height) / static_cast<float>(GLHelper::prevHeight);

    if (!GLHelper::ptr_window)
    {
        std::cerr << "GLFW unable to create OpenGL context - abort program\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(GLHelper::ptr_window);

    glfwSetFramebufferSizeCallback(GLHelper::ptr_window, GLHelper::fbsize_cb);
    glfwSetKeyCallback(GLHelper::ptr_window, GLHelper::key_cb);
    glfwSetMouseButtonCallback(GLHelper::ptr_window, GLHelper::mousebutton_cb);
    glfwSetCursorPosCallback(GLHelper::ptr_window, GLHelper::mousepos_cb);
    glfwSetScrollCallback(GLHelper::ptr_window, GLHelper::mousescroll_cb);

    // this is the default setting ...
    glfwSetInputMode(GLHelper::ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Part 2: Initialize entry points to OpenGL functions and extensions
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Unable to initialize GLEW - error: "
            << glewGetErrorString(err) << " abort program" << std::endl;
        return false;
    }

    if (GLEW_VERSION_4_4)
    {
        std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
    }
    else
    {
        std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
        return false;
    }

    return true;
}

/*  _________________________________________________________________________ */
/*! cleanup

@param none

@return none

For now, there are no resources allocated by the application program.
The only task is to have GLFW return resources back to the system and
gracefully terminate.
*/
void GLHelper::cleanup()
{
  // Part 1
  glfwTerminate();
}

void GLHelper::print_specs()
{
  GLubyte const* str = glGetString(GL_VENDOR);
  std::cout << "GPU Vendor: " << str << std::endl;

  str = glGetString(GL_RENDERER);
  std::cout << "GL Renderer: " << str << std::endl;

  str = glGetString(GL_VERSION);
  std::cout << "GL Version: " << str << std::endl;

  str = glGetString(GL_SHADING_LANGUAGE_VERSION);
  std::cout << "GL Shader Version: " << str << std::endl;

  GLint value;
  glGetIntegerv(GL_MAJOR_VERSION, &value);
  std::cout << "GL Major Version: " << value << std::endl;

  glGetIntegerv(GL_MINOR_VERSION, &value);
  std::cout << "GL Minor Version: " << value << std::endl;

  glGetIntegerv(GL_DOUBLEBUFFER, &value);
  const char* buffer;
  if (value)
  {
    buffer = "double";
  }
  else
  {
    buffer = "single";
  }
  std::cout << "Current OpenGL Context is " << buffer << " buffered" << std::endl;

  glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &value);
  std::cout << "Maximum Vertex Count: " << value << std::endl;

  glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &value);
  std::cout << "Maximum Indices Count: " << value << std::endl;

  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
  std::cout << "GL Maximum texture size: " << value << std::endl;

  glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &value);
  std::cout << "GL Maximum array texture layers: " << value << std::endl;

  GLint viewDim[2];
  glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &viewDim[0]);
  std::cout << "Maximum Viewport Dimensions: " << viewDim[0] << " x " << viewDim[1] << std::endl;
}

/*  _________________________________________________________________________*/
/*! key_cb

@param GLFWwindow*
Handle to window that is receiving event

@param int
the keyboard key that was pressed or released

@parm int
Platform-specific scancode of the key

@parm int
GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
action will be GLFW_KEY_UNKNOWN if GLFW lacks a key token for it,
for example E-mail and Play keys.

@parm int
bit-field describing which modifier keys (shift, alt, control)
were held down

@return none

This function is called when keyboard buttons are pressed.
When the ESC key is pressed, the close flag of the window is set.
*/
void GLHelper::key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod) {

  (void)mod;
  (void)scancode;
  (void)pwin;

  //float g_dt = static_cast<float>(GLHelper::update_time(1.0));

  if (GLFW_PRESS == action)
  {
#ifdef _DEBUG
    std::cout << "Key pressed" << std::endl;
#endif
  }
  else if (GLFW_REPEAT == action)
  {
#ifdef _DEBUG
    std::cout << "Key repeatedly pressed" << std::endl;
#endif
  }
  else if (GLFW_RELEASE == action)
  {
#ifdef _DEBUG
    std::cout << "Key released" << std::endl;
#endif
  }
}

/*  _________________________________________________________________________*/
/*! mousebutton_cb

@param GLFWwindow*
Handle to window that is receiving event

@param int
the mouse button that was pressed or released
GLFW_MOUSE_BUTTON_LEFT and GLFW_MOUSE_BUTTON_RIGHT specifying left and right
mouse buttons are most useful

@parm int
action is either GLFW_PRESS or GLFW_RELEASE

@parm int
bit-field describing which modifier keys (shift, alt, control)
were held down

@return none

This function is called when mouse buttons are pressed.
*/
void GLHelper::mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod)
{
  (void)mod;
  (void)pwin;

  switch (button)
  {
  case GLFW_MOUSE_BUTTON_LEFT:
#ifdef _DEBUG
    std::cout << "Left mouse button ";
#endif
    break;
  case GLFW_MOUSE_BUTTON_RIGHT:
#ifdef _DEBUG
    std::cout << "Right mouse button ";
#endif
    break;
  }

  switch (action)
  {
  case GLFW_PRESS:
#ifdef _DEBUG
    std::cout << "pressed!!!" << std::endl;
#endif
    break;
  case GLFW_RELEASE:
#ifdef _DEBUG
    std::cout << "released!!!" << std::endl;
#endif
    break;
  }

}

/*  _________________________________________________________________________*/
/*! mousepos_cb

@param GLFWwindow*
Handle to window that is receiving event

@param double
new cursor x-coordinate, relative to the left edge of the client area

@param double
new cursor y-coordinate, relative to the top edge of the client area

@return none

This functions receives the cursor position, measured in screen coordinates but
relative to the top-left corner of the window client area.
*/
void GLHelper::mousepos_cb(GLFWwindow* pwin, double xpos, double ypos)
{
#ifdef _DEBUG
  std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
#endif
  (void)pwin;
  (void)xpos;
  (void)ypos;
  GLHelper::m_posX = xpos;
  GLHelper::m_posY = ypos;
}

/*  _________________________________________________________________________*/
/*! mousescroll_cb

@param GLFWwindow*
Handle to window that is receiving event

@param double
Scroll offset along X-axis

@param double
Scroll offset along Y-axis

@return none

This function is called when the user scrolls, whether with a mouse wheel or
touchpad gesture. Although the function receives 2D scroll offsets, a simple
mouse scroll wheel, being vertical, provides offsets only along the Y-axis.
*/
void GLHelper::mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset)
{
#ifdef _DEBUG
  std::cout << "Mouse scroll wheel offset: ("
    << xoffset << ", " << yoffset << ")" << std::endl;
#endif
  (void)pwin;
  (void)xoffset;
  (void)yoffset;
}

/*  _________________________________________________________________________ */
/*! error_cb

@param int
GLFW error code

@parm char const*
Human-readable description of the code

@return none

The error callback receives a human-readable description of the error and
(when possible) its cause.
*/
void GLHelper::error_cb(int error, char const* description)
{
  if (description != nullptr)
  {
    std::cerr << "GLFW error: " << description << std::endl;
  }
  else
  {
    error = 0;
  }
}

/*  _________________________________________________________________________ */
/*! fbsize_cb

@param GLFWwindow*
Handle to window that is being resized

@parm int
Width in pixels of new window size

@parm int
Height in pixels of new window size

@return none

This function is called when the window is resized - it receives the new size
of the window in pixels.
*/
void GLHelper::fbsize_cb(GLFWwindow* ptr_win, int _width, int _height)
{
  (void)ptr_win;
  std::cout << "fbsize_cb getting called!!!" << std::endl;
  // use the entire framebuffer as drawing region
  glViewport(0, 0, _width, _height);
  // later, if working in 3D, we'll have to set the projection matrix here ...
}

/*  _________________________________________________________________________*/
/*! update_time

@param double&
fps: computed frames per second

@param double
fps_calc_interval: the interval (in seconds) at which fps is to be
calculated

@return double
Return time interval (in seconds) between previous and current frames

This function is first called in init() and once each game loop by update().
It uses GLFW's time functions to:
1. compute interval in seconds between each frame
2. compute the frames per second every "fps_calc_interval" seconds
*/
double GLHelper::update_time(double fps_calc_interval)
{
  // get elapsed time (in seconds) between previous and current frames
  static double prev_time = glfwGetTime();
  double curr_time = glfwGetTime();
  double delta_time = curr_time - prev_time;
  prev_time = curr_time;

  // fps calculations
  static double count = 0.0; // number of game loop iterations
  static double start_time = glfwGetTime();
  // get elapsed time since very beginning (in seconds) ...
  double elapsed_time = curr_time - start_time;

  ++count;

  // update fps at least every 10 seconds ...
  fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
  fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;

  if (elapsed_time > fps_calc_interval)
  {
    GLHelper::fps = count / elapsed_time;
    start_time = curr_time;
    count = 0.0;
  }

  deltaTime = delta_time;

  // done calculating fps ...
  return delta_time;
}