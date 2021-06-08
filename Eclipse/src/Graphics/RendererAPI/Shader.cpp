#include "pch.h"
#include "Shader.h"

Shader::Shader():
	handle{0}, isLinked{GL_FALSE}
{
}

GLboolean Shader::CompileLinkValidate(std::vector<std::pair<GLenum, std::string>> shaderFiles)
{
	for (auto& elem : shaderFiles)
	{
		if (CompileShader(elem.first, elem.second.c_str()) == GL_FALSE)
		{
			return GL_FALSE;
		}
	}

	if (Link() == GL_FALSE)
	{
		return GL_FALSE;
	}

	if (Validate() == GL_FALSE)
	{
		return GL_FALSE;
	}

	return GL_TRUE;
}

void Shader::Use()
{
	if (handle > 0 && isLinked == GL_TRUE)
	{
		glUseProgram(handle);
	}
}

void Shader::UnUse()
{
	glUseProgram(0);
}

std::string Shader::GetLog() const
{
	return message;
}

GLboolean Shader::IsLinked() const
{
	return isLinked;
}

GLboolean Shader::GetHandle() const
{
	return static_cast<GLboolean>(handle);
}

GLint Shader::GetUniformLocation(const GLchar* name)
{
	return glGetUniformLocation(handle, name);
}

void Shader::DeleteShaderProgram()
{
	if (handle > 0)
	{
		glDeleteProgram(handle);
	}
}

void Shader::SetUniform(const GLchar* name, GLboolean value)
{
	GLint location = glGetUniformLocation(handle, name);

	if (location >= 0) 
	{
		glUniform1i(location, value);
	}
	else 
	{
		std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
	}
}

void Shader::SetUniform(const GLchar* name, GLint value)
{
	GLint location = glGetUniformLocation(handle, name);

	if (location >= 0)
	{
		glUniform1i(location, value);
	}
	else
	{
		std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
	}
}

void Shader::SetUniform(const GLchar* name, GLfloat value)
{
	GLint location = glGetUniformLocation(handle, name);

	if (location >= 0)
	{
		glUniform1f(location, value);
	}
	else
	{
		std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
	}
}

void Shader::SetUniform(const GLchar* name, GLfloat x, GLfloat y)
{
	GLint location = glGetUniformLocation(handle, name);

	if (location >= 0)
	{
		glUniform2f(location, x, y);
	}
	else
	{
		std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
	}
}

void Shader::SetUniform(const GLchar* name, GLfloat x, GLfloat y, GLfloat z)
{
	GLint location = glGetUniformLocation(handle, name);

	if (location >= 0)
	{
		glUniform3f(location, x, y, z);
	}
	else
	{
		std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
	}
}

void Shader::SetUniform(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	GLint location = glGetUniformLocation(handle, name);

	if (location >= 0)
	{
		glUniform4f(location, x, y, z, w);
	}
	else
	{
		std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
	}
}

void Shader::SetUniform(const GLchar* name, glm::vec2& value)
{
	GLint location = glGetUniformLocation(handle, name);

	if (location >= 0)
	{
		glUniform2f(location, value.x, value.y);
	}
	else
	{
		std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
	}
}

void Shader::SetUniform(const GLchar* name, glm::vec3& value)
{
	GLint location = glGetUniformLocation(handle, name);

	if (location >= 0)
	{
		glUniform3f(location, value.x, value.y, value.z);
	}
	else
	{
		std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
	}
}

void Shader::SetUniform(const GLchar* name, glm::vec4& value)
{
	GLint location = glGetUniformLocation(handle, name);

	if (location >= 0)
	{
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	else
	{
		std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
	}
}

void Shader::SetUniform(const GLchar* name, glm::mat3& value)
{
	GLint location = glGetUniformLocation(handle, name);

	if (location >= 0)
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
	}
	else
	{
		std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
	}
}

void Shader::SetUniform(const GLchar* name, glm::mat4& value)
{
	GLint location = glGetUniformLocation(handle, name);

	if (location >= 0)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	}
	else
	{
		std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
	}
}

std::string Shader::GetShaderData(const std::string& fileName)
{
	if (FileExists(fileName) == GL_FALSE)
	{
		message = "File does not exist.";
		return std::string();
	}

	if (handle <= 0)
	{
		handle = glCreateProgram();

		if (handle == 0)
		{
			message = "Failed to create program handle.";
			return std::string();
		}
	}

	std::ifstream shaderFile(fileName, std::ifstream::in);

	if (!shaderFile)
	{
		message = "Error opening file " + fileName;
		return std::string();
	}

	std::stringstream ss;
	ss << shaderFile.rdbuf();
	shaderFile.close();

	return ss.str();
}

GLboolean Shader::CompileShader(GLenum shaderType, const std::string fileName)
{
	std::string shaderData = GetShaderData(fileName);

	if (shaderData.empty())
	{
		return GL_FALSE;
	}

	GLuint shaderHandle = 0;

	switch (shaderType)
	{
	case VERTEX_SHADER: 
		shaderHandle = glCreateShader(GL_VERTEX_SHADER);
		break;
	case FRAGMENT_SHADER: 
		shaderHandle = glCreateShader(GL_FRAGMENT_SHADER); 
		break;
	case GEOMETRY_SHADER: 
		shaderHandle = glCreateShader(GL_GEOMETRY_SHADER); 
		break;
	case TESS_CONTROL_SHADER: 
		shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER); 
		break;
	case TESS_EVALUATION_SHADER:
		shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER); 
		break;
	default:
		message = "Invalid shader type.";
		return GL_FALSE;
	}

	const GLchar* data[] = { shaderData.c_str() };
	glShaderSource(shaderHandle, 1, data, NULL);

	glCompileShader(shaderHandle);

	GLint status;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, &errorLog[0]);

		message = errorLog.data();

		return GL_FALSE;
	}
	else
	{
		glAttachShader(handle, shaderHandle);
		return GL_TRUE;
	}
}

GLboolean Shader::FileExists(const std::string& fileName)
{
	std::ifstream inputFile(fileName);

	return inputFile.good();
}

GLboolean Shader::Link()
{
	if (isLinked == GL_TRUE)
	{
		return GL_TRUE;
	}

	if (handle <= 0)
	{
		return GL_FALSE;
	}

	glLinkProgram(handle);

	GLint status;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		message = "Failed to link shader program.";
		return GL_FALSE;
	}
	else
	{
		isLinked = GL_TRUE;
		return GL_TRUE;
	}
}

GLboolean Shader::Validate()
{
	if (handle <= 0 || isLinked == GL_FALSE)
	{
		return GL_FALSE;
	}

	glValidateProgram(handle);

	GLint status;
	glGetProgramiv(handle, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
	{
		message = "Failed to validate shader programe for current OpenGL context.";
		return GL_FALSE;
	}
	else
	{
		return GL_TRUE;
	}
}

int Shader::GetLocation(const char* name) const
{
	const int location = glGetUniformLocation(static_cast<GLboolean>(handle), name);
	if (location == -1)
	{
	  std::cout << "[OpenGL Error] Shader: Uniform " << name << " does not exist!" << std::endl;
	  std::exit(EXIT_FAILURE);
	}

	return location;
}