#pragma once

#include <Engine/Renderer/Model.h>
#include <Engine/Renderer/Mesh.h>

#include <reactphysics3d/reactphysics3d.h>

#include <glm/glm.hpp>

#include <vector>

namespace dyxide
{
	class Collider
	{
	public:
		Collider(reactphysics3d::Collider* collider) : m_Collider(collider) {};
		virtual ~Collider() = default;

		reactphysics3d::Collider* GetCollider() { return m_Collider; }

	private:
		reactphysics3d::Collider* m_Collider;
	};

	class CollisionShape
	{
	public:
		virtual reactphysics3d::CollisionShape* GetCollisionShape() = 0;

		virtual glm::vec3 GetScale() = 0;
		virtual void SetScale(glm::vec3 scale) = 0;
	};

	class MeshCollisionShape : public CollisionShape
	{
	public:
		MeshCollisionShape(std::vector<glm::vec3> vertices);
		~MeshCollisionShape();

		virtual reactphysics3d::CollisionShape* GetCollisionShape() override
		{
			return m_ConvexMeshShape;
		}

		virtual glm::vec3 GetScale() override
		{
			auto& s = m_ConvexMeshShape->getScale();
			return { s.x, s.y, s.z };
		}

		virtual void SetScale(glm::vec3 scale) override
		{
			m_ConvexMeshShape->setScale({ scale.x, scale.y, scale.z });
		}

		static Ref<MeshCollisionShape> Create(Ref<Model> model);
		static Ref<MeshCollisionShape> Create(std::vector<Mesh> meshes);
		static Ref<MeshCollisionShape> Create(std::vector<Vertex> vertices);

	private:
		reactphysics3d::ConvexMesh* m_ConvexMesh;
		reactphysics3d::ConvexMeshShape* m_ConvexMeshShape;
	};
}