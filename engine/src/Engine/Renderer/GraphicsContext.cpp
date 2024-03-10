#include <dxpch.h>
#include <glad/glad.h>
#include <Engine/Renderer/GraphicsContext.h>

#include <Engine/Core/Log.h>
#include <Engine/Renderer/Renderer.h>

#include <glfw/glfw3.h>

namespace dyxide
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		return CreateScope<GraphicsContext>(static_cast<GLFWwindow*>(window));
	}

	GraphicsContext::GraphicsContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		DYXIDE_ASSERT(windowHandle, "Window handle is null!");
	}

	void GraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DYXIDE_ASSERT(status, "Failed to initialize Glad!");

		DYXIDE_INFO("OpenGL Info:");
		DYXIDE_INFO("Vendor: " << glGetString(GL_VENDOR));
		DYXIDE_INFO("Renderer: " << glGetString(GL_RENDERER));
		DYXIDE_INFO("Version: " << glGetString(GL_VERSION));

		DYXIDE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "dyxide requires at least OpenGL version 4.5!");
	}

	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}