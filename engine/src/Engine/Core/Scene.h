#pragma once

#include <Engine/Events/Event.h>
#include <Engine/Renderer/Camera.h>
#include <Engine/Core/Timestep.h>
#include <Engine/Core/UUID.h>

#include <entt.hpp>

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

class b2World;

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

		void OnViewportResize(uint32_t width, uint32_t height);

		Entity DuplicateEntity(Entity entity);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUUID(UUID uuid);

		Entity GetPrimaryCameraEntity();

		bool IsPaused() const { return m_IsPaused; }
		void SetPaused(bool paused) { m_IsPaused = paused; }

		void Step(int frames = 1);

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

	protected:
		virtual void OnLogicUpdate(Timestep ts) = 0;
		virtual void OnRenderUI(Timestep ts) = 0;

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsPaused = false;
		int m_StepFrames = 0;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
