#include <Game/MenuScene.h>

#include <Engine/Core/Application.h>
#include <Engine/Core/Entity.h>
#include <Engine/Core/Components.h>
#include <Engine/Core/ResourceLoader.h>
#include <Engine/Renderer/RendererUI.h>

namespace dyxide
{
	Entity g_Sun;
	Entity g_Title;

	void MenuScene::OnLoad()
	{
		auto camera = CreateEntity("Camera");
		camera.AddComponent<CameraComponent>().Primary = true;

		g_Title = CreateEntity("Title");
		g_Title.AddComponent<TextComponent>("Dyxide");

		g_Sun = CreateEntity("Sun");
		g_Sun.AddComponent<SpriteRendererComponent>().Texture = ResourceLoader::LoadTexture2D("textures/Sun.png");

		auto& window = ::dyxide::Application::Get().GetWindow();
		OnViewportResize(window.GetWidth(), window.GetHeight());
	}

	void MenuScene::OnUnload()
	{

	}

	void MenuScene::OnViewportResize(uint32_t width, uint32_t height)
	{
		glm::vec3 center{ width / 2, height / 2, 0 };
		g_Title.GetComponent<TransformComponent>().Translation = { center - glm::vec3(175, 55, 0) };
		g_Sun.GetComponent<TransformComponent>().Translation = { center + glm::vec3(-50, 100, 0) };
	}

	void MenuScene::OnLogicUpdate(Timestep ts)
	{

	}

	void MenuScene::OnEvent(Event& e)
	{

	}
}