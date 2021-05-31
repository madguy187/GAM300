/**********************************************************************************
* \file         Shader.h
*
* \brief        Definition of functions of Shader.cpp
*
* \author       Lin Yanping Rachel
*
* \email		    l.yanpingrachel@digipen.edu
*
* \date			    9 October 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*               or disclosure of this file or its contents without the prior
*               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef SHADER_H
#define SHADER_H

#include "GLEW/include/GL/glew.h"
#include "GLM/glm/glm.hpp"

class Shader
{
public:
	Shader();

	GLboolean CompileLinkValidate(std::vector<std::pair<GLenum, std::string>> shaderFiles);

	void Use();
	void UnUse();

	std::string GetLog() const;
	GLboolean IsLinked() const;
	GLboolean GetHandle() const;
	GLint GetUniformLocation(const GLchar* name);

	void DeleteShaderProgram();

	void SetUniform(const GLchar* name, GLboolean value);
	void SetUniform(const GLchar* name, GLint value);
	void SetUniform(const GLchar* name, GLfloat value);
	void SetUniform(const GLchar* name, GLfloat x, GLfloat y);
	void SetUniform(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
	void SetUniform(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void SetUniform(const GLchar* name, glm::vec2& value);
	void SetUniform(const GLchar* name, glm::vec3& value);
	void SetUniform(const GLchar* name, glm::vec4& value);
	void SetUniform(const GLchar* name, glm::mat3& value);
	void SetUniform(const GLchar* name, glm::mat4& value);
	int GetLocation(const char* name) const;
private:
	std::string GetShaderData(const std::string& fileName);
	GLboolean CompileShader(GLenum shaderType, const std::string fileName);
	GLboolean FileExists(const std::string& fileName);
	GLboolean Link();
	GLboolean Validate();

	enum ShaderType
	{
		VERTEX_SHADER = GL_VERTEX_SHADER,
		FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
		TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
		TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER
	};

	GLuint handle;
	GLboolean isLinked;
	std::string message;
	unsigned int m_id;
};

#endif /* SHADER_H */
