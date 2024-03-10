#pragma once

#include <Engine/Core/Common.h>

struct GLFWwindow;

namespace dyxide
{
	class GraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow* windowHandle);
		virtual ~GraphicsContext() = default;

		void Init();
		void SwapBuffers();

		static Scope<GraphicsContext> Create(void* window);

	private:
		GLFWwindow* m_WindowHandle;
	};
}