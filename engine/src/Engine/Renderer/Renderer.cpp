#include <dxpch.h>
#include <Engine/Renderer/Renderer.h>

#include <Engine/Core/Application.h>
#include <Engine/Core/Window.h>
#include <Engine/Core/ResourceLoader.h>
#include <Engine/Renderer/VertexArray.h>
#include <Engine/Renderer/Shader.h>
#include <Engine/Renderer/UniformBuffer.h>
#include <Engine/Renderer/RenderCommand.h>
#include <Engine/Math/Math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <array>

namespace dyxide
{
	struct RendererData
	{
		Ref<Texture2D> WhiteTexture;
		Ref<Shader> MeshShader;
		Renderer::Statistics Stats;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		s_Data.WhiteTexture = Texture2D::Create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.MeshShader = ResourceLoader::LoadShader("shaders/Renderer_Mesh.vertex", "shaders/Renderer_Mesh.fragment");
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{

	}

	void Renderer::BeginScene(CameraComponent& camera, TransformComponent& cameraTransform)
	{
		auto& window = Application::Get().GetWindow();
		s_Data.MeshShader->Bind();
		s_Data.MeshShader->SetMat4("u_Projection", glm::perspective(glm::radians(camera.FieldOfView), static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()), camera.PerspectiveNear, camera.PerspectiveFar));
		s_Data.MeshShader->SetMat4("u_View", glm::lookAt(cameraTransform.Translation, cameraTransform.Translation + Math::Vector3Forward(cameraTransform.Rotation), Math::Vector3Up(cameraTransform.Rotation)));
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::DrawModel(const glm::mat4& transform, ModelRendererComponent& mrc)
	{
		if (!mrc.ModelAsset)
		{
			DYXIDE_WARN("No ModelAsset provided for ModelRendererComponent selected");
			return;
		}

		s_Data.MeshShader->Bind();
		s_Data.MeshShader->SetMat4("u_Transform", transform);

		for (Mesh& mesh : mrc.ModelAsset->GetMeshes())
		{
			s_Data.MeshShader->SetInt("u_TextureDiffuse", 0);
			if (mesh.GetDiffuseTexture())
			{
				mesh.GetDiffuseTexture()->Bind(0);
			}
			else
			{
				s_Data.WhiteTexture->Bind(0);
			}

			RenderCommand::DrawIndexed(mesh.GetVertexArray());
			mesh.GetVertexArray()->Unbind();
			s_Data.Stats.DrawCalls++;
		}

		s_Data.MeshShader->Unbind();
		s_Data.Stats.ModelCount++;
	}

	void Renderer::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer::Statistics Renderer::GetStats()
	{
		return s_Data.Stats;
	}
}
