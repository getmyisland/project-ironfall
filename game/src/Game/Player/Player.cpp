#include <Game/Player/Player.h>

#include <Engine/Core/ResourceLoader.h>

namespace dyxide
{
	Player::Player(Scene& scene) : m_Scene(scene)
	{
		m_Camera = m_Scene.CreateEntity("Player_Camera");
		m_Camera.AddComponent<CameraComponent>().Primary = true;

		m_Model = m_Scene.CreateEntity("Player_Model");
		auto model = ResourceLoader::LoadModel("/models/Humanoid.obj");
		m_Model.AddComponent<ModelRendererComponent>().ModelAsset = model;
		auto& transform = m_Model.GetComponent<TransformComponent>();
		transform.Rotation = glm::angleAxis(180.0f, glm::vec3(0, 1, 0));
		auto& rigidbody = m_Model.AddComponent<RigidBodyComponent>();
		rigidbody.BodyType = RigidBodyComponent::RigidBodyType::Dynamic;
		rigidbody.AngularLockAxisConstraints = glm::vec3(0, 1, 0); // Only allow rotation on Y axis
		m_Model.AddComponent<ColliderComponent>().Shape = MeshCollisionShape::Create(model);
	}

	Player::~Player()
	{

	}

	void Player::Update(Timestep ts)
	{
		auto& camera = m_Camera.GetComponent<TransformComponent>();
		auto& model = m_Model.GetComponent<TransformComponent>();
		m_Controller.CalculateMovement(ts, camera, model);
	}

	const Ref<Player> Player::Create(Scene& scene)
	{
		return CreateRef<Player>(scene);
	}
}