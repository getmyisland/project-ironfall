#pragma once

#include <Engine/Core/Common.h>
#include <Engine/Core/UUID.h>
#include <Engine/Renderer/Model.h>
#include <Engine/Renderer/Texture.h>
#include <Engine/Renderer/Font.h>
#include <Engine/Physics/Collider.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <vector>

namespace dyxide
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID id)
			: ID(id)
		{
		}
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag)
		{
		}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f }; // Euler Angles
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		bool IsActive{ true };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation)
		{
		}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct CameraComponent
	{
		bool Primary = false;
		float FieldOfView = 45.0f;
		float PerspectiveNear = 0.01f, PerspectiveFar = 1000.0f;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct ModelRendererComponent
	{
		Ref<Model> ModelAsset;

		ModelRendererComponent() = default;
		ModelRendererComponent(const ModelRendererComponent&) = default;
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct TextComponent
	{
		std::string TextString;
		Ref<Font> FontAsset = Font::GetDefault();
		glm::vec4 Color{ 1.0f };
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;

		TextComponent() = default;
		TextComponent(const TextComponent&) = default;
		TextComponent(const std::string& textString)
			: TextString(textString) {}
	};

	struct ColliderComponent
	{
		glm::vec3 Position{ 0.0f };
		glm::vec3 Scale{ 1.0f };

		Ref<CollisionShape> Shape;
	};

	struct RigidBodyComponent
	{
		enum class RigidBodyType { Static = 0, Kinematic, Dynamic };
		RigidBodyType BodyType = RigidBodyType::Static;

		Ref<Collider> Col;

		bool UseGravity{ true };
		bool CanSleep{ true };

		glm::vec3 Force{ 0.0f };

		// Mass in kg
		float Mass{ 1.0f };

		// 1 = motion is not restricted on this axis
		// 0 = motion is restricted on this axis
		glm::vec3 LinearLockAxisConstraints{ 1, 1, 1 };
		glm::vec3 AngularLockAxisConstraints{ 1, 1, 1 };

		void* RigidBody;
	};

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents =
		ComponentGroup<TransformComponent, CameraComponent, SpriteRendererComponent, ModelRendererComponent, TextComponent, ColliderComponent, RigidBodyComponent>;
}
