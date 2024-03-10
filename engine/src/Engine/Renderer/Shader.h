#pragma once

#include <Engine/Core/Common.h>

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace dyxide
{
	class Shader
	{
	public:
		Shader(uint32_t id);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat4(const std::string& name, const glm::mat4& value);

		static bool Exists(const std::string& name);
		static Ref<Shader> Get(const std::string& name);

		static Ref<Shader> LoadFromFile(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
		static Ref<Shader> Compile(const std::string& name, const char* vertexSrc, const char* fragmentSrc);

	private:
		uint32_t m_RendererID;

	private:
		static std::unordered_map<std::string, Ref<Shader>> s_Cache;
	};
}
