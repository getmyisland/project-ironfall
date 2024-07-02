#pragma once

#include <Engine/Renderer/VertexArray.h>

#include <glm/glm.hpp>

#include <vector>

namespace dyxide
{
	struct PrimitiveVertex
	{
		glm::vec3 Position;
		glm::vec3 Color;
	};

	enum class PrimitiveType
	{
		TRIANGLE,
		LINE
	};

	class Primitive
	{
	public:
		Primitive(const PrimitiveType type, std::vector<PrimitiveVertex>& vertices);

		const PrimitiveType GetType() { return m_Type; }

		const Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }

		const std::vector<PrimitiveVertex>& GetVertices() { return m_Vertices; }

	private:
		const PrimitiveType m_Type;

		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;

		std::vector<PrimitiveVertex> m_Vertices;
	};
}