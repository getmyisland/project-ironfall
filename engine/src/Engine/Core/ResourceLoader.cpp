#include <Engine/Core/Log.h>
#include <Engine/Core/ResourceLoader.h>

#include <Engine/Core/Assert.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <filesystem>

namespace dyxide
{
	Ref<Shader> ResourceLoader::LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderFile.open(GetResourceDir() + vertexPath);
			fShaderFile.open(GetResourceDir() + fragmentPath);

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
			DYXIDE_ERROR("Error: " << e.what());
			return nullptr;
		}

		return Shader::Compile(name, vertexCode.c_str(), fragmentCode.c_str());
	}

	Ref<Texture2D> ResourceLoader::LoadTexture2D(const std::string& path)
	{
		return Texture2D::Create(GetResourceDir() + path);
	}

	Ref<Font> ResourceLoader::LoadFont(const std::string& path)
	{
		return Font::Create(GetResourceDir() + path);
	}

	std::string ResourceLoader::GetCurrentDir()
	{
		try
		{
			return std::filesystem::current_path().string();
		}
		catch (const std::exception& e)
		{
			DYXIDE_ASSERT(false, "Failed to retrieve current directory: " + std::string(e.what()));
			return "";
		}
	}

	std::string ResourceLoader::GetResourceDir()
	{
		std::string currentDir = GetCurrentDir();
		if (!currentDir.empty())
		{
			return currentDir + "/resources/";
		}
		else
		{
			DYXIDE_ASSERT(false, "Failed to retrieve resource directory");
			return "";
		}
	}
}