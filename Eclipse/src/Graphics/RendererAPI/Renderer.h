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

#ifdef DEBUG_OPENGL
// Call glGetError first, then calls function 'x', then GLLogError
#define GLCall(x) glGetError(); x; Renderer::GLLogError(#x, __FILE__, __LINE__);
#else
// Empty wrapper
#define GLCall(x) x
#endif 