#pragma once

#include <Engine/Core/Common.h>
#include <Engine/Core/Components.h>
#include <Engine/Renderer/RenderCommand.h>
#include <Engine/Renderer/Shader.h>

namespace dyxide
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(CameraComponent& camera, TransformComponent& cameraTransform);
		static void EndScene();

		static void DrawModel(const glm::mat4& transform, ModelRendererComponent& mrc);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t ModelCount = 0;
		};
		static void ResetStats();
		static Statistics GetStats();
	};
}
