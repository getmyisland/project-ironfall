#include <dxpch.h>
#include <glad/glad.h>

#include <Engine/Renderer/Shader.h>

#include <Engine/Core/Timer.h>
#include <Engine/Renderer/Renderer.h>

#include <Engine/Core/Common.h>
#include <Engine/Core/Assert.h>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

namespace dyxide
{
	Shader::Shader(uint32_t id) : m_RendererID(id)
	{

	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	static void CheckCompileErrors(unsigned int shader, uint32_t type)
	{
		int success;
		char infoLog[1024];
		if (type != GL_PROGRAM)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				DYXIDE_ERROR((type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader linking failed:\n" << infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				DYXIDE_ERROR("Program Linking failed.\n" << infoLog);
			}
		}
	}

	Ref<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			if (!vShaderFile.is_open() || !fShaderFile.is_open())
			{
				throw std::runtime_error("Failed to open shader files");
			}

			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			DYXIDE_ERROR("Shader file not successfully read:\n" << e.what()
				<< "\nVertex shader path: " << vertexPath
				<< "\nFragment shader path: " << fragmentPath
			);
			return nullptr;
		}
		catch (const std::runtime_error& e)
		{
			DYXIDE_ERROR(e.what());
			return nullptr;
		}

		return Shader::Compile(vertexCode.c_str(), fragmentCode.c_str());
	}

	Ref<Shader> Shader::Compile(const char* vertexSrc, const char* fragmentSrc)
	{
		unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSrc, NULL);
		glCompileShader(vertex);
		CheckCompileErrors(vertex, GL_VERTEX_SHADER);

		unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSrc, NULL);
		glCompileShader(fragment);
		CheckCompileErrors(fragment, GL_FRAGMENT_SHADER);

		unsigned int id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);
		CheckCompileErrors(id, GL_PROGRAM);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return CreateRef<Shader>(id);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}