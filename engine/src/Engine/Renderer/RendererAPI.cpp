#include <dxpch.h>
#include <glad/glad.h>
#include <Engine/Core/Log.h>
#include <Engine/Renderer/RendererAPI.h>

namespace dyxide
{
	Scope<RendererAPI> RendererAPI::Create()
	{
		return CreateScope<RendererAPI>();
	}

	void MessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:  return;       DYXIDE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM: return;      DYXIDE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW: return;         DYXIDE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: return; DYXIDE_TRACE(message); return;
		}

		DYXIDE_ASSERT(false, "Unknown severity level!");
	}

	void RendererAPI::Init()
	{
#ifdef DYXIDE_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(MessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void RendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
}