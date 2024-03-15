#include <Game/MenuScene.h>

#include <Engine/Core/Application.h>
#include <Engine/Core/Entity.h>
#include <Engine/Core/Components.h>
#include <Engine/Core/ResourceLoader.h>
#include <Engine/Renderer/RendererUI.h>

namespace dyxide
{
	Sprite m_Sprite;

	void MenuScene::OnLoad()
	{
		auto camera = CreateEntity("camera");
		camera.AddComponent<CameraComponent>().Primary = true;

		m_Sprite.Texture = ResourceLoader::LoadTexture2D("textures/Sun.png");
	}

	void MenuScene::OnUnload()
	{

	}

	void MenuScene::OnLogicUpdate(Timestep ts)
	{

	}

	void MenuScene::OnRenderUI(Timestep ts)
	{
		auto& window = ::dyxide::Application::Get().GetWindow();
		auto width = window.GetWidth();
		auto height = window.GetHeight();

		glm::vec3 center{ width / 2, height / 2, 0 };
		RendererUI::DrawSprite(m_Sprite, { center + glm::vec3(-50, 100, 0) });
		RendererUI::DrawString("Dyxide", { center - glm::vec3(175, 55, 0) });
	}

	void MenuScene::OnEvent(Event& e)
	{

	}
}