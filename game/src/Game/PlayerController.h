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

		void CalculateMovement(const Timestep& ts, TransformComponent& camera, TransformComponent& model);

	private:
		const float m_MovementSpeed = 10.0f;
		const float m_JumpPower = 10.0f;
		const float m_MouseSensitivity = 0.05f;
		float m_CameraYaw = 0.0f;
		float m_CameraPitch = 0.0f;

		bool m_IsFirstMouseInput = true;
		glm::vec2 m_LastMousePos = glm::vec2();
	};
}