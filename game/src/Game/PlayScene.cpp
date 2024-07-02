#include <Game/PlayScene.h>

#include <Engine/Core/Application.h>
#include <Engine/Core/Entity.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/ResourceLoader.h>

#include <Game/PlayerController.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <sstream>

namespace dyxide
{
	Entity g_PlayerCamera;
	PlayerController g_PlayerController;

	Entity g_PlayerModel;
	Entity g_Ground;
	Entity g_Debug;

	void PlayScene::OnLoad()
	{
		Input::LockCursor();

		g_PlayerCamera = CreateEntity("Player_Camera");
		g_PlayerCamera.AddComponent<CameraComponent>().Primary = true;

		g_PlayerModel = CreateEntity("Player_Model");
		auto model = ResourceLoader::LoadModel("/models/Humanoid.obj");
		g_PlayerModel.AddComponent<ModelRendererComponent>().ModelAsset = model;
		auto& transform = g_PlayerModel.GetComponent<TransformComponent>();
		transform.Rotation = glm::angleAxis(180.0f, glm::vec3(0, 1, 0));
		auto& rigidbody = g_PlayerModel.AddComponent<RigidBodyComponent>();
		rigidbody.BodyType = RigidBodyComponent::RigidBodyType::Dynamic;
		rigidbody.AngularLockAxisConstraints = glm::vec3(0, 1, 0); // Only allow rotation on Y axis
		//g_PlayerModel.AddComponent<ColliderComponent>().Shape = BoxCollisionShape::Create({ 5, 5, 5 }); // TODO size isn't applied correctly for unknown reasons
		g_PlayerModel.AddComponent<ColliderComponent>().Shape = MeshCollisionShape::Create(model);

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

		auto& camera = g_PlayerCamera.GetComponent<TransformComponent>();
		auto& model = g_PlayerModel.GetComponent<TransformComponent>();
		g_PlayerController.CalculateMovement(ts, camera, model);

		std::stringstream oss;
		oss << "position: " << glm::to_string(camera.Translation) << "\n";
		oss << "rotation: " << glm::to_string(camera.Rotation) << "\n";
		oss << "model position: " << glm::to_string(model.Translation) << "\n";
		oss << "model rotation: " << glm::to_string(model.Rotation);

		g_Debug.GetComponent<TextComponent>().TextString = oss.str();
	}

	void PlayScene::OnViewportResize(uint32_t width, uint32_t height)
	{
		g_Debug.GetComponent<TransformComponent>().Translation = { glm::vec3(10, height - 20, 0) };
	}
}