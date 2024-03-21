#include <dxpch.h>
#include <Engine/Core/Input.h>

#include <Engine/Core/Application.h>

#include <GLFW/glfw3.h>

#include <cstdint>

namespace dyxide
{
	static bool s_IsFirstMouseInput{ true };
	static bool s_MouseCallbackThisFrame{ false };
	static glm::vec2 s_LastMousePos{ glm::vec2() };
	static glm::vec2 s_MouseOffset{ glm::vec2() };

	static void GLFWMouseCallback(GLFWwindow* window, double yPosIn, double xPosIn)
	{
		float xPos = static_cast<float>(-xPosIn);
		float yPos = static_cast<float>(-yPosIn);

		if (s_IsFirstMouseInput)
		{
			s_LastMousePos = glm::vec2(xPos, yPos);
			s_IsFirstMouseInput = false;
		}

		float x_offset = xPos - s_LastMousePos.x;
		float y_offset = s_LastMousePos.y - yPos; // Reversed since y-Coordinates go from bottom to top

		s_LastMousePos = glm::vec2(xPos, yPos);
		s_MouseOffset = glm::vec2(x_offset, y_offset);

		s_MouseCallbackThisFrame = true;
	}

	void Input::Init()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetCursorPosCallback(window, GLFWMouseCallback);
	}

	bool Input::IsKeyPressed(const KeyCode key)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	static void ResetMouseOffset()
	{
		if (s_MouseCallbackThisFrame)
		{
			s_MouseCallbackThisFrame = false;
		}
		else
		{
			s_MouseOffset = glm::vec2();
		}
	}

	glm::vec2 Input::GetMouseOffset()
	{
		ResetMouseOffset();

		return s_MouseOffset;
	}

	glm::vec2 Input::GetMousePosition()
	{
		return s_LastMousePos;
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}

	void Input::ShowCursor()
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Input::HideCursor()
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void Input::UnlockCursor()
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Input::LockCursor()
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}