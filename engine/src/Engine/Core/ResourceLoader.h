#pragma once

#include <Engine/Core/Common.h>
#include <Engine/Renderer/Model.h>
#include <Engine/Renderer/Shader.h>
#include <Engine/Renderer/Texture.h>
#include <Engine/Renderer/Font.h>

#include <string>

namespace dyxide
{
	class ResourceLoader
	{
	public:
		static Ref<Shader> LoadShader(const std::string& vertexPath, const std::string& fragmentPath);
		static Ref<Texture2D> LoadTexture2D(const std::string& path);
		static Ref<Model> LoadModel(const std::string& path);
		static Ref<Font> LoadFont(const std::string& path);

	private:
		static std::string GetCurrentDir();
		static std::string GetResourceDir();
	};
}