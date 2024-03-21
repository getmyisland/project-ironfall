#pragma once

#include <Engine/Core/Entity.h>

#include <glm/glm.hpp>

namespace dyxide
{
	class PlayerController
	{
	public:
		PlayerController() = default;
		virtual ~PlayerController() = default;

		void CalculateMovement(const Timestep& ts, TransformComponent& transform);

	private:
		const float m_MovementSpeed = 10.0f;
		const float m_MouseSensitivity = 0.05f;

		bool m_IsFirstMouseInput = true;
		glm::vec2 m_LastMousePos = glm::vec2();
	};
}