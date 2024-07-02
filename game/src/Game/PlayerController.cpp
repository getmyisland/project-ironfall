#include <Game/PlayerController.h>

#include <Engine/Core/Log.h>
#include <Engine/Core/Input.h>
#include <Engine/Math/Math.h>

namespace dyxide
{
	void PlayerController::CalculateMovement(const Timestep& ts, TransformComponent& camera, TransformComponent& model)
	{
		// Movement
		float velocity = m_MovementSpeed * ts;
		glm::vec3 movement(0.0f);

		if (Input::IsKeyPressed(Key::W))
			movement += model.GetForward();
		if (Input::IsKeyPressed(Key::S))
			movement -= model.GetForward();
		if (Input::IsKeyPressed(Key::A))
			movement -= model.GetRight();
		if (Input::IsKeyPressed(Key::D))
			movement += model.GetRight();

		// Normalize movement vector if it's not zero
		if (glm::length(movement) > 0.0f)
			movement = glm::normalize(movement);

		model.Translation += movement * velocity;

        // Rotation
        glm::vec2 mouseOffset = Input::GetMouseOffset();
        float yaw = -glm::radians(mouseOffset.y * m_MouseSensitivity);
        float pitch = glm::radians(mouseOffset.x * m_MouseSensitivity);

        // Update camera yaw and pitch
        m_CameraYaw += yaw;
        m_CameraPitch += pitch;

        // Clamp pitch to avoid flipping
        m_CameraPitch = glm::clamp(m_CameraPitch, -glm::half_pi<float>() * 0.9f, glm::half_pi<float>() * 0.9f);

        // Rotate the model only around the Y-axis (yaw)
        model.Rotation = glm::quat(glm::vec3(0.0f, m_CameraYaw, 0.0f));

        // Calculate camera rotation (pitch and yaw only)
        //glm::quat cameraRotation = glm::quat(glm::vec3(m_CameraPitch, m_CameraYaw, 0.0f));

		// Calculate the camera's position relative to the model
		glm::vec3 upOffset = glm::vec3(0.0f, 2.0f, 0.0f);
		glm::vec3 backwardOffset = -model.GetForward() * 2.0f;
		glm::vec3 leftOffset = model.GetRight() * 1.0f;

		glm::vec3 cameraPosition = model.Translation + upOffset + backwardOffset + leftOffset;

		// Apply smooth interpolation to camera position
		float smoothFactor = 1.0f; // Adjust this value to control smoothness
		camera.Translation = glm::mix(camera.Translation, cameraPosition, smoothFactor);

		// Calculate and apply camera rotation
		glm::quat cameraRotation = glm::quat(glm::vec3(m_CameraPitch, m_CameraYaw, 0.0f));
		camera.Rotation = glm::slerp(camera.Rotation, cameraRotation, smoothFactor);
	}
}