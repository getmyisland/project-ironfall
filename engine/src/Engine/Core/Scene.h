#pragma once

#include <Engine/Events/Event.h>
#include <Engine/Core/Timestep.h>
#include <Engine/Core/UUID.h>

#include <reactphysics3d/reactphysics3d.h>

#include <entt.hpp>

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

namespace dyxide
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);

		virtual void OnLoad() = 0;
		virtual void OnUnload() = 0;
		virtual void OnEvent(Event& e) = 0;

		void OnWindowResize(uint32_t width, uint32_t height);

		Entity DuplicateEntity(Entity entity);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUUID(UUID uuid);

		Entity GetPrimaryCameraEntity();

		bool IsPaused() const { return m_IsPaused; }
		void SetPaused(bool paused) { m_IsPaused = paused; }

		reactphysics3d::PhysicsCommon* GetPhysicsCommon() { return &m_PhysicsCommon; }

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

	protected:
		virtual void OnLogicUpdate(Timestep ts) = 0;
		virtual void OnViewportResize(uint32_t width, uint32_t height) = 0;

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		template<typename T>
		void OnComponentRemoved(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		reactphysics3d::PhysicsCommon m_PhysicsCommon;
		reactphysics3d::PhysicsWorld* m_PhysicsWorld;
		reactphysics3d::DebugRenderer* m_DebugRenderer;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsPaused = false;

		const float m_PhysicsTimeStep = 1.0f / 60.0f;
		double m_PhysicsAccumulator = 0;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
