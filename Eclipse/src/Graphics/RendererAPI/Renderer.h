#pragma once
#include "glew.h"
#include <iostream>

namespace Eclipse
{

  class Renderer
  {
  public:
    // Prints the error message to console
    static void GLLogError(const char* p_function, const char* p_file, int p_line)
    {
      const GLenum error = glGetError();
      if (error != GL_NO_ERROR)
      {
        std::cout << "[OpenGL Error] (" << error << "): " << p_function << " " << p_file << ":" << p_line << std::endl;
      }
    }

  };
}

#define GLCall(x) x
 