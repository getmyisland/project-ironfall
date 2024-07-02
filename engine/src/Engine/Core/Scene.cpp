#include <dxpch.h>
#include <Engine/Core/Scene.h>

#include <Engine/Core/Entity.h>
#include <Engine/Core/Components.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Renderer/RenderCommand.h>
#include <Engine/Renderer/RendererUI.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>

#include <tuple>

namespace dyxide
{
	Scene::Scene()
	{
		reactphysics3d::PhysicsWorld::WorldSettings settings;
		settings.defaultVelocitySolverNbIterations = 20;
		settings.isSleepingEnabled = false;
		settings.gravity = reactphysics3d::Vector3(0, -9.81, 0);
		m_PhysicsWorld = m_PhysicsCommon.createPhysicsWorld(settings);
		m_PhysicsWorld->setIsDebugRenderingEnabled(true);

		// Get a reference to the debug renderer
		m_DebugRenderer = &m_PhysicsWorld->getDebugRenderer();

		// Select the contact points and contact normals to be displayed
		m_DebugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
		//m_DebugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
		m_DebugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
		m_DebugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
		//m_DebugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE_NORMAL, true);
	}

	Scene::~Scene() { }

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto view = src.view<Component>();
				for (auto srcEntity : view)
				{
					entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

					auto& srcComponent = src.get<Component>(srcEntity);
					dst.emplace_or_replace<Component>(dstEntity, srcComponent);
				}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update game state and variables
		OnLogicUpdate(ts);

		if (!m_IsPaused)
		{
			m_PhysicsAccumulator += ts;

			while (m_PhysicsAccumulator >= m_PhysicsTimeStep)
			{
				// Update the rigid body transform with the transform component values
				// Sync the rigid body's properties with the rigid body's component properties
				{
					auto view = m_Registry.view<TransformComponent, RigidBodyComponent>();
					for (auto entity : view)
					{
						auto [transform, rbc] = view.get<TransformComponent, RigidBodyComponent>(entity);

						auto* rb = static_cast<reactphysics3d::RigidBody*>(rbc.RigidBody);
						if (!rb)
						{
							DYXIDE_WARN("Unable to retrieve RigidBody from Component");
							continue;
						}

						rb->setType(static_cast<reactphysics3d::BodyType>(rbc.BodyType));
						rb->enableGravity(rbc.UseGravity);
						rb->setIsAllowedToSleep(rbc.CanSleep);
						rb->setMass(rbc.Mass);
						rb->setLinearLockAxisFactor({ rbc.LinearLockAxisConstraints.x, rbc.LinearLockAxisConstraints.y, rbc.LinearLockAxisConstraints.z });
						rb->setAngularLockAxisFactor({ rbc.AngularLockAxisConstraints.x, rbc.AngularLockAxisConstraints.y, rbc.AngularLockAxisConstraints.z });

						rb->applyWorldForceAtCenterOfMass({ rbc.Force.x, rbc.Force.y, rbc.Force.z });

						if (m_Registry.has<ColliderComponent>(entity))
						{
							auto& cc = m_Registry.get<ColliderComponent>(entity);
							if (cc.Shape && cc.Shape->GetCollisionShape())
							{
								if (cc.Shape->GetScale() != cc.Scale)
								{
									cc.Shape->SetScale(cc.Scale);
								}

								if (!rbc.Col || !rbc.Col->GetCollider() || !rbc.Col->GetCollider()->getCollisionShape() || rbc.Col->GetCollider()->getCollisionShape() != cc.Shape->GetCollisionShape())
								{
									if (rbc.Col && rbc.Col->GetCollider())
									{
										rb->removeCollider(rbc.Col->GetCollider());
									}

									auto* collider = rb->addCollider(cc.Shape->GetCollisionShape(), reactphysics3d::Transform::identity());
									rbc.Col = CreateRef<Collider>(collider);
								}
							}

							if (rbc.Col && rbc.Col->GetCollider())
							{
								reactphysics3d::Transform trans;
								trans.setPosition({ cc.Position.x, cc.Position.y , cc.Position.z });
								rbc.Col->GetCollider()->setLocalToBodyTransform(trans);
							}
						}

						reactphysics3d::Transform trans;
						trans.setPosition({ transform.Translation.x, transform.Translation.y , transform.Translation.z });
						auto quat = reactphysics3d::Quaternion::fromEulerAngles({ transform.Rotation.x, transform.Rotation.y , transform.Rotation.z });
						trans.setOrientation(quat);
						rb->setTransform(trans);
					}
				}

				m_PhysicsWorld->update(m_PhysicsTimeStep);

				// Update the transform component with the transform of the rigid body
				{
					auto view = m_Registry.view<TransformComponent, RigidBodyComponent>();
					for (auto entity : view)
					{
						auto [transform, rbc] = view.get<TransformComponent, RigidBodyComponent>(entity);

						auto* rb = static_cast<reactphysics3d::RigidBody*>(rbc.RigidBody);
						if (!rb)
						{
							DYXIDE_WARN("Unable to retrieve RigidBody from Component");
							continue;
						}

						rbc.Force = glm::vec3();

						const reactphysics3d::Transform& trans = rb->getTransform();
						transform.Translation = { trans.getPosition().x, trans.getPosition().y, trans.getPosition().z };
						transform.Rotation = { trans.getOrientation().getVectorV().x, trans.getOrientation().getVectorV().y, trans.getOrientation().getVectorV().z };
					}
				}

				m_PhysicsAccumulator -= m_PhysicsTimeStep;
			}
		}

		// Try finding the main camera
		CameraComponent* primaryCamera = nullptr;
		TransformComponent* primaryCameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					primaryCamera = &camera;
					primaryCameraTransform = &transform;
					break;
				}
			}
		}

		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
		RenderCommand::Clear();

		if (primaryCamera && primaryCameraTransform)
		{
			Renderer::BeginScene(*primaryCamera, *primaryCameraTransform);

			// Draw models
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<ModelRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, model] = group.get<TransformComponent, ModelRendererComponent>(entity);

					if (!transform.IsActive) continue;

					Renderer::DrawModel(transform.GetTransform(), model);
				}
			}

			Renderer::EndScene();
		}
		else
		{
			DYXIDE_WARN("No Primary Camera found");
		}

		// Draw debug physics shapes
		{
			// Convert all lines into vertices
			std::vector<PrimitiveVertex> lines;
			for (int i = 0; i < m_DebugRenderer->getNbLines(); ++i) {
				auto line = m_DebugRenderer->getLinesArray()[i];
				lines.push_back({ glm::vec3(line.point1.x, line.point1.y, line.point1.z),
									 glm::vec3((line.color1 >> 24) & 0xFF, (line.color1 >> 16) & 0xFF, (line.color1 >> 8) & 0xFF) });
				lines.push_back({ glm::vec3(line.point2.x, line.point2.y, line.point2.z),
									 glm::vec3((line.color2 >> 24) & 0xFF, (line.color2 >> 16) & 0xFF, (line.color2 >> 8) & 0xFF) });
			}

			// Create a line primitive from the vertices and draw it
			Primitive pline = Primitive(PrimitiveType::LINE, lines);
			Renderer::DrawPrimitive(pline);

			// Convert all triangles into vertices
			std::vector<PrimitiveVertex> triangles;
			for (int i = 0; i < m_DebugRenderer->getNbTriangles(); ++i) {
				auto triangle = m_DebugRenderer->getTrianglesArray()[i];
				triangles.push_back({ glm::vec3(triangle.point1.x, triangle.point1.y, triangle.point1.z),
									 glm::vec3((triangle.color1 >> 24) & 0xFF, (triangle.color1 >> 16) & 0xFF, (triangle.color1 >> 8) & 0xFF) });
				triangles.push_back({ glm::vec3(triangle.point2.x, triangle.point2.y, triangle.point2.z),
									 glm::vec3((triangle.color2 >> 24) & 0xFF, (triangle.color2 >> 16) & 0xFF, (triangle.color2 >> 8) & 0xFF) });
				triangles.push_back({ glm::vec3(triangle.point3.x, triangle.point3.y, triangle.point3.z),
									 glm::vec3((triangle.color3 >> 24) & 0xFF, (triangle.color3 >> 16) & 0xFF, (triangle.color3 >> 8) & 0xFF) });
			}

			// Create a triangle primitive from the vertices and draw it
			Primitive ptriangle = Primitive(PrimitiveType::TRIANGLE, triangles);
			Renderer::DrawPrimitive(ptriangle);
		}

		RendererUI::BeginScene();

		// Draw sprites
		{
			auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);

				if (!transform.IsActive) continue;

				// NOTE: Sprites are manually upscaled by 100 or else it would be almost invisible at the default scale.
				RendererUI::DrawSprite(transform.GetTransform() * glm::scale(glm::mat4(1.0f), glm::vec3{ 100.0f }), sprite);
			}
		}

		// Draw text
		{
			auto view = m_Registry.view<TransformComponent, TextComponent>();
			for (auto entity : view)
			{
				auto [transform, text] = view.get<TransformComponent, TextComponent>(entity);

				if (!transform.IsActive) continue;

				// NOTE: Text is manually upscaled by 100 or else it would be almost invisible at the default scale.
				RendererUI::DrawString(text.TextString, transform.GetTransform() * glm::scale(glm::mat4(1.0f), glm::vec3{ 100.0f }), text);
			}
		}

		RendererUI::EndScene();
	}

	void Scene::OnWindowResize(uint32_t width, uint32_t height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		OnViewportResize(width, height);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		// Copy name because we're going to modify component data structure
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}

	Entity Scene::FindEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		//static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<RigidBodyComponent>(Entity entity, RigidBodyComponent& component)
	{
		// Populating values for the transform at this point is useless as it will be done nonetheless when updating physics.
		reactphysics3d::Transform transform;
		reactphysics3d::RigidBody* rb = m_PhysicsWorld->createRigidBody(transform);
		rb->setIsDebugEnabled(true);
		component.RigidBody = rb;
	}

	template<typename T>
	void Scene::OnComponentRemoved(Entity entity, T& component)
	{
		//static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentRemoved<RigidBodyComponent>(Entity entity, RigidBodyComponent& component)
	{
		auto* rb = static_cast<reactphysics3d::RigidBody*>(component.RigidBody);
		if (!rb)
		{
			DYXIDE_WARN("Unable to retrieve RigidBody from Component");
			return;
		}

		m_PhysicsWorld->destroyRigidBody(rb);
	}
}
