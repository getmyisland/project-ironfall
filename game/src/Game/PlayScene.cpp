#include <Game/PlayScene.h>

#include <Engine/Core/Application.h>
#include <Engine/Core/Entity.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/ResourceLoader.h>

#include <Game/Player/Player.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <sstream>

namespace dyxide
{
	Entity g_Ground;
	Entity g_Debug;

	void PlayScene::OnLoad()
	{
		Input::LockCursor();

		m_Player = Player::Create(*this);

		g_Ground = CreateEntity("Ground");
		auto ground = ResourceLoader::LoadModel("/models/Ground.obj");
		g_Ground.AddComponent<ModelRendererComponent>().ModelAsset = ground;
		auto& gt = g_Ground.GetComponent<TransformComponent>();
		gt.Translation = { 0.0f, -5.0f, 0.0f };
		g_Ground.AddComponent<RigidBodyComponent>();
		g_Ground.AddComponent<ColliderComponent>().Shape = MeshCollisionShape::Create(ground);

		g_Debug = CreateEntity("Debug");
		g_Debug.AddComponent<TextComponent>("");
		g_Debug.GetComponent<TransformComponent>().Scale = glm::vec3{ 0.2f };

		auto& window = ::dyxide::Application::Get().GetWindow();
		OnViewportResize(window.GetWidth(), window.GetHeight());
	}

	void PlayScene::OnUnload()
	{
	}

	void PlayScene::OnEvent(Event& e)
	{
	}

	void PlayScene::OnLogicUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(Key::Escape))
		{
			Application::Get().Close();
			return;
		}

		m_Player->Update(ts);

		std::stringstream oss;
		//oss << "position: " << glm::to_string(m_Player->GetCamera()->Translation) << "\n";
		//oss << "rotation: " << glm::to_string(camera.Rotation) << "\n";
		//oss << "model position: " << glm::to_string(model.Translation) << "\n";
		//oss << "model rotation: " << glm::to_string(model.Rotation);

		g_Debug.GetComponent<TextComponent>().TextString = oss.str();
	}

	void PlayScene::OnViewportResize(uint32_t width, uint32_t height)
	{
		g_Debug.GetComponent<TransformComponent>().Translation = { glm::vec3(10, height - 20, 0) };
	}
}