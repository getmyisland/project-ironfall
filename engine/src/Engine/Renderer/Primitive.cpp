#include <dxpch.h>
#include <Engine/Renderer/Primitive.h>

namespace dyxide
{
	Primitive::Primitive(const PrimitiveType type, std::vector<PrimitiveVertex>& vertices) : m_Type(type), m_Vertices(vertices)
	{
		m_VertexArray = CreateRef<VertexArray>();
		m_VertexArray->Bind();

		m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(PrimitiveVertex));
		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Color" }
			});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		m_VertexArray->Unbind();
	}
}