#pragma once

#include <Engine/Core/Common.h>

#include <Game/Player/PlayerController.h>
#include <Game/Player/PlayerWeaponManager.h>

namespace dyxide
{
	class Player
	{
	public:
		Player(Scene& scene);
		~Player();

		void Update(Timestep ts);

		const Entity* const GetCamera() { return &m_Camera; }
		const Entity* const GetModel() { return &m_Model; }

		static const Ref<Player> Create(Scene& scene);

	private:
		Scene& m_Scene;

		Entity m_Camera;
		Entity m_Model;

		PlayerController m_Controller;
		PlayerWeaponManager m_WeaponManager;
	};
}