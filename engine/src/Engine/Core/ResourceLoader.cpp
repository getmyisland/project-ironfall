#include <Engine/Core/Log.h>
#include <Engine/Core/ResourceLoader.h>

#include <Engine/Core/Assert.h>

#include <exception>
#include <filesystem>

namespace dyxide
{
	Ref<Shader> ResourceLoader::LoadShader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		return Shader::Create(GetResourceDir() + vertexPath, GetResourceDir() + fragmentPath);
	}

	Ref<Texture2D> ResourceLoader::LoadTexture2D(const std::string& path)
	{
		return Texture2D::Create(GetResourceDir() + path);
	}

	Ref<Model> ResourceLoader::LoadModel(const std::string& path)
	{
		return Model::Create(GetResourceDir() + path);
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