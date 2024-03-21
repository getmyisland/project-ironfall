#include <Game/PlayerController.h>

#include <Engine/Core/Input.h>
#include <Engine/Math/Math.h>

namespace dyxide
{
	void PlayerController::CalculateMovement(const Timestep& ts, TransformComponent& transform)
	{
		if (Input::IsKeyPressed(Key::R))
		{
			transform.Translation = glm::vec3();
			transform.Rotation = glm::vec3();
		}

		float velocity = m_MovementSpeed * ts;

		if (Input::IsKeyPressed(Key::W))
		{
			transform.Translation += Math::Vector3Forward(transform.Rotation) * velocity;
		}
		if (Input::IsKeyPressed(Key::S))
		{
			transform.Translation -= Math::Vector3Forward(transform.Rotation) * velocity;
		}
		if (Input::IsKeyPressed(Key::A))
		{
			transform.Translation -= Math::Vector3Right(transform.Rotation) * velocity;
		}
		if (Input::IsKeyPressed(Key::D))
		{
			transform.Translation += Math::Vector3Right(transform.Rotation) * velocity;
		}

		glm::vec2 mouseOffset = Input::GetMouseOffset();
		mouseOffset.x *= m_MouseSensitivity;
		mouseOffset.y *= m_MouseSensitivity;

		transform.Rotation += glm::vec3(mouseOffset.x, mouseOffset.y, 0);

		// Make sure that when x-Rotation is out of bounds, the screen doesn't get flipped
		if (transform.Rotation.x > 89.0f)
		{
			transform.Rotation.x = 89.0f;
		}
		else if (transform.Rotation.x < -89.0f)
		{
			transform.Rotation.x = -89.0f;
		}
	}
}