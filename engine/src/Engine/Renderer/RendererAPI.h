#pragma once

#include <Engine/Renderer/VertexArray.h>

#include <glm/glm.hpp>

namespace dyxide
{
	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;

		void Init();
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void SetClearColor(const glm::vec4& color);
		void Clear();

		void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0);
		void DrawLine(const Ref<VertexArray>& vertexArray, size_t vertexCount);
		void DrawTriangle(const Ref<VertexArray>& vertexArray, size_t vertexCount);

		static Scope<RendererAPI> Create();
	};
}
