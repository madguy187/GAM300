#include "pch.h"
#include "OpenGL_Context.h"

using namespace Eclipse;

GLFWwindow* OpenGL_Context::ptr_window;
GLint OpenGL_Context::width;
GLint OpenGL_Context::height;
GLdouble OpenGL_Context::m_posX;
GLdouble OpenGL_Context::m_posY;
float OpenGL_Context::windowRatioX;
float OpenGL_Context::windowRatioY;
GLdouble OpenGL_Context::fps;
GLdouble OpenGL_Context::deltaTime;

static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod)
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

static void on_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  auto pWindow = static_cast<OpenGL_Context*>(glfwGetWindowUserPointer(window));
  pWindow->on_key(key, scancode, action, mods);
}

void Eclipse::OpenGL_Context::on_key(int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS)
  {
  }
}

static void on_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  std::cout << "Mouse scroll wheel offset: (" << xoffset << ", " << yoffset << ")" << std::endl;
}

static void on_window_size_callback(GLFWwindow* window, int width, int height)
{
  auto pWindow = static_cast<OpenGL_Context*>(glfwGetWindowUserPointer(window));
  pWindow->on_resize(width, height);
}

void Eclipse::OpenGL_Context::on_resize(int width, int height)
{
  width = width;
  height = height;

  glViewport(0, 0, width, height);
}

static void on_window_close_callback(GLFWwindow* window)
{
  OpenGL_Context::on_close();
}

void Eclipse::OpenGL_Context::on_close()
{
  glfwSetWindowShouldClose(OpenGL_Context::ptr_window, GLFW_TRUE);
}

void Eclipse::OpenGL_Context::mousepos_cb(GLFWwindow* pwin, double xpos, double ypos)
{
#ifdef _DEBUG
  std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
#endif
  (void)pwin;
  (void)xpos;
  (void)ypos;
  m_posX = xpos;
  m_posX = ypos;
}

void error_cb(int error, char const* description)
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

void mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset)
{
#ifdef _DEBUG
  std::cout << "Mouse scroll wheel offset: ("
    << xoffset << ", " << yoffset << ")" << std::endl;
#endif
  (void)pwin;
  (void)xoffset;
  (void)yoffset;
}

void Eclipse::OpenGL_Context::fbsize_cb(GLFWwindow* ptr_win, int _width, int _height)
{
  (void)ptr_win;
  std::cout << "fbsize_cb getting called!!!" << std::endl;
  // use the entire framebuffer as drawing region
  glViewport(0, 0, _width, _height);
  // later, if working in 3D, we'll have to set the projection matrix here ...
}

void Eclipse::OpenGL_Context::key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod) {

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

void Eclipse::OpenGL_Context::mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod)
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

void Eclipse::OpenGL_Context::LoadConfigData(std::string configFile)
{
  Parser input;
  input.ParseFile(configFile);

  title = input.doc["title"].GetString();
  width = input.doc["dimensions"][rapidjson::SizeType(0)].GetInt();
  height = input.doc["dimensions"][rapidjson::SizeType(1)].GetInt();

}

bool Eclipse::OpenGL_Context::init(std::string configFile)
{
  LoadConfigData(configFile);

  // Part 1
  if (!glfwInit())
  {
    std::cout << "GLFW init has failed - abort program!!!" << std::endl;
    return false;
  }

  // In case a GLFW function fails, an error is reported to callback function
  //glfwSetErrorCallback(error_cb);

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

  OpenGL_Context::prevWidth = OpenGL_Context::width;
  OpenGL_Context::prevHeight = OpenGL_Context::height;

  // Create the window and store this window as window pointer
  // so that we can use it in callback functions
  OpenGL_Context::ptr_window = glfwCreateWindow(width, height, title.c_str(), NULL(), NULL);
  OpenGL_Context::windowRatioX = static_cast<float>(OpenGL_Context::width) / static_cast<float>(OpenGL_Context::prevWidth);
  OpenGL_Context::windowRatioY = static_cast<float>(OpenGL_Context::height) / static_cast<float>(OpenGL_Context::prevHeight);

  if (!OpenGL_Context::ptr_window)
  {
    std::cerr << "GLFW unable to create OpenGL context - abort program\n";
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(OpenGL_Context::ptr_window);
  glfwSetKeyCallback(OpenGL_Context::ptr_window, on_key_callback);
  glfwSetScrollCallback(OpenGL_Context::ptr_window, on_scroll_callback);
  glfwSetFramebufferSizeCallback(OpenGL_Context::ptr_window, on_window_size_callback);
  glfwSetMouseButtonCallback(OpenGL_Context::ptr_window, OpenGL_Context::mousebutton_cb);
  glfwSetCursorPosCallback(OpenGL_Context::ptr_window, OpenGL_Context::mousepos_cb);
  glfwSetWindowSizeCallback(OpenGL_Context::ptr_window, on_window_size_callback);
  glfwSetWindowCloseCallback(OpenGL_Context::ptr_window, on_window_close_callback);

  // this is the default setting ...
  glfwSetInputMode(OpenGL_Context::ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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

  glEnable(GL_DEPTH_TEST);

  return true;
}

void Eclipse::OpenGL_Context::print_specs()
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

void Eclipse::OpenGL_Context::pre_render()
{
  glViewport(0, 0, width, height);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Graphics::init();
}

void Eclipse::OpenGL_Context::post_render()
{
  glfwPollEvents();
  glfwSwapBuffers(OpenGL_Context::ptr_window);
}

void Eclipse::OpenGL_Context::end()
{
  glfwDestroyWindow(OpenGL_Context::ptr_window);
  glfwTerminate();
}


double Eclipse::OpenGL_Context::update_time(double fps_calc_interval)
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
        OpenGL_Context::fps = count / elapsed_time;
        start_time = curr_time;
        count = 0.0;
    }

    deltaTime = delta_time;

    // done calculating fps ...
    return delta_time;
}

//GLint Eclipse::OpenGL_Context::GetWindowWidth()
//{
//  return width;
//}
//
//GLint Eclipse::OpenGL_Context::GetWindowHeight()
//{
//  return height;
//}
//
//void* Eclipse::OpenGL_Context::GetWindow()
//{
//  return ptr_window;
//}