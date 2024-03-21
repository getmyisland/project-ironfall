#include <dxpch.h>
#include <Engine/Renderer/Mesh.h>

namespace dyxide
{
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, Ref<Texture2D>& diffuseTexture) : m_Vertices(vertices), m_Indices(indices), m_DiffuseTexture(diffuseTexture)
	{
		m_VertexArray = CreateRef<VertexArray>();
		m_VertexArray->Bind();

		m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Tangent" },
			{ ShaderDataType::Float3, "a_Bitangent" },
			{ ShaderDataType::Float4, "a_BoneIDs" },
			{ ShaderDataType::Float4, "a_Weights" }
			});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer = IndexBuffer::Create(indices.data(), indices.size());
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_VertexArray->Unbind();
	}
}