#pragma once

#include <Engine/Renderer/Texture.h>
#include <Engine/Renderer/VertexArray.h>

#include <glm/glm.hpp>

#include <vector>

#define MAX_BONE_INFLUENCE 4

namespace dyxide
{
	struct ModelVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		int BoneIDs[MAX_BONE_INFLUENCE];
		float Weights[MAX_BONE_INFLUENCE];
	};

	class Mesh
	{
	public:
		Mesh(std::vector<ModelVertex>& vertices, std::vector<uint32_t>& indices, Ref<Texture2D>& diffuseTexture);

		const Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }

		const Ref<Texture2D>& GetDiffuseTexture() { return m_DiffuseTexture; }

		const std::vector<ModelVertex>& GetVertices() { return m_Vertices; }

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		std::vector<ModelVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		Ref<Texture2D> m_DiffuseTexture;
	};
}