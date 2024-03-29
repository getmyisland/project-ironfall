#include <dxpch.h>
#include <Engine/Physics/Collider.h>

#include <Engine/Core/Application.h>

#include <algorithm>

namespace dyxide
{
	/////////////////////////////////////////////////////////////////////////////
	// MeshCollisionShape ///////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	MeshCollisionShape::MeshCollisionShape(std::vector<glm::vec3> vertices)
	{
		rp3d::VertexArray vertexArray(vertices.data(), 3 * sizeof(float),
			vertices.size(), rp3d::VertexArray::DataType::VERTEX_FLOAT_TYPE);

		std::vector<rp3d::Message> messages;
		m_ConvexMesh = Application::Get().GetScene()->GetPhysicsCommon()->createConvexMesh(vertexArray, messages);

		if (messages.size() > 0)
		{
			for (const rp3d::Message& message : messages)
			{
				switch (message.type) {
				case rp3d::Message::Type::Information:
					DYXIDE_INFO(message.text);
					break;
				case rp3d::Message::Type::Warning:
					DYXIDE_WARN(message.text);
					break;
				case rp3d::Message::Type::Error:
					DYXIDE_ERROR(message.text);
					break;
				}
			}
		}

		DYXIDE_ASSERT(m_ConvexMesh, "ConvexMesh Creation failed.");

		// Create the ConvexMeshShape 
		m_ConvexMeshShape = Application::Get().GetScene()->GetPhysicsCommon()->createConvexMeshShape(m_ConvexMesh, {1, 1, 1});
	}

	MeshCollisionShape::~MeshCollisionShape()
	{

	}

	Ref<MeshCollisionShape> MeshCollisionShape::Create(Ref<Model> model)
	{
		return Create(model->GetMeshes());
	}

	Ref<MeshCollisionShape> MeshCollisionShape::Create(std::vector<Mesh> meshes)
	{
		std::vector<Vertex> concatenatedVertices;

		size_t totalVerticesCount = 0;
		for (auto& mesh : meshes) {
			totalVerticesCount += mesh.GetVertices().size();
		}
		concatenatedVertices.reserve(totalVerticesCount);

		for (auto& mesh : meshes) {
			concatenatedVertices.insert(concatenatedVertices.end(), mesh.GetVertices().begin(), mesh.GetVertices().end());
		}

		return Create(concatenatedVertices);
	}

	Ref<MeshCollisionShape> MeshCollisionShape::Create(std::vector<Vertex> vertices)
	{
		std::vector<glm::vec3> positions;
		positions.reserve(vertices.size());

		std::transform(vertices.begin(), vertices.end(), std::back_inserter(positions),
			[](const Vertex& vertex) { return vertex.Position; });

		return CreateRef<MeshCollisionShape>(positions);
	}
}