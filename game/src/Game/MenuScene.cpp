#include <Game/MenuScene.h>

#include <Engine/Core/Application.h>
#include <Engine/Core/Entity.h>
#include <Engine/Core/Components.h>
#include <Engine/Renderer/RendererUI.h>

namespace dyxide
{
	void MenuScene::OnLoad()
	{
		auto camera = CreateEntity("camera");
		camera.AddComponent<CameraComponent>().Primary = true;
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
		glm::vec3 centerPos = glm::vec3((width / 2) - (26 * 3), (height / 2) - (26 / 2), 0);
		RendererUI::DrawString("Dyxide", centerPos);
	}

	void MenuScene::OnEvent(Event& e)
	{

	}
}