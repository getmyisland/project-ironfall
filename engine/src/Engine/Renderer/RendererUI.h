#pragma once

#include <Engine/Renderer/Font.h>
#include <Engine/Renderer/Texture.h>

#include <glm/glm.hpp>

#include <cstdint>

namespace dyxide
{
	struct Sprite
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;

		Sprite() = default;
		Sprite(const Sprite&) = default;
		Sprite(const glm::vec4& color)
			: Color(color)
		{
		}
	};

	class RendererUI
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene();
		static void EndScene();
		static void Flush();

		static void DrawSprite(Sprite& sprite, const glm::vec3& position, const glm::vec3& scale = glm::vec3 { 100.0f });

		struct TextParams
		{
			Ref<Font> Font = GetDefaultFont();
			glm::vec4 Color{ 1.0f };
			float Scale = 26.0f;
			float Kerning = 0.0f;
			float LineSpacing = 0.0f;
		};
		static void DrawString(const std::string& string, const glm::vec3& position, const TextParams& textParams = TextParams());

		static Ref<Font> GetDefaultFont();

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void StartBatch();
		static void NextBatch();
	};
}
