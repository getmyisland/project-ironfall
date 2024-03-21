#pragma once

#include <Engine/Core/Components.h>
#include <Engine/Renderer/Font.h>
#include <Engine/Renderer/Texture.h>

#include <glm/glm.hpp>

#include <cstdint>

namespace dyxide
{
	class RendererUI
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene();
		static void EndScene();
		static void Flush();

		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src);

		struct TextParams
		{
			glm::vec4 Color{ 1.0f };
			float Kerning = 0.0f;
			float LineSpacing = 0.0f;
		};
		static void DrawString(const std::string& string, Ref<Font> font, const glm::mat4& transform, const TextParams& textParams);
		static void DrawString(const std::string& string, const glm::mat4& transform, const TextComponent& component);

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
