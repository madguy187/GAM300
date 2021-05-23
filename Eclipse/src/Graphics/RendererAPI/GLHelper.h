/**********************************************************************************
* \file            GLHelper.h
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
/* !
This file contains the declaration of namespace Helper that encapsulates the
functionality required to create an OpenGL context using GLFW; use GLEW
to load OpenGL extensions; initialize OpenGL state; and finally initialize
the OpenGL application by calling initalization functions associated with
objects participating in the application.

*//*__________________________________________________________________________*/

#ifndef GLHELPER_H
#define GLHELPER_H

#include "Graphics.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "Parser.h"

struct GLHelper
 //GLHelper structure to encapsulate initialization stuff ...
{
	static void LoadConfigData(std::string configFile);

	static bool init(std::string configFile);
	static void cleanup();
	static void print_specs();
  
	// callbacks 
	static void error_cb(int error, char const* description);
	static void fbsize_cb(GLFWwindow *ptr_win, int _width, int _height);

	// I/O callbacks
	static void key_cb(GLFWwindow *pwin, int key, int scancode, int action, int mod);
	static void mousebutton_cb(GLFWwindow *pwin, int button, int action, int mod);
	static void mousescroll_cb(GLFWwindow *pwin, double xoffset, double yoffset);
	static void mousepos_cb(GLFWwindow *pwin, double xpos, double ypos);

	static double update_time(double CalcInt = 1.0);

	static GLint width, height;
	static GLint prevWidth, prevHeight;
	static float windowRatioX, windowRatioY;
	static GLdouble fps;
	static std::string title;
	static GLFWwindow *ptr_window;
	static GLdouble m_posX, m_posY;
	static GLdouble deltaTime;
};

#endif /* GLHELPER_H */
