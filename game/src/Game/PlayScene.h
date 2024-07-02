#pragma once

#include <Engine/Core/Common.h>
#include <Engine/Core/Scene.h>
#include <Engine/Core/Timestep.h>

#include <Game/Player/Player.h>

namespace dyxide
{
	class PlayScene : public Scene
	{
	public:
		virtual void OnLoad() override;
		virtual void OnUnload() override;
		virtual void OnEvent(Event& e) override;

	protected:
		virtual void OnLogicUpdate(Timestep ts) override;
		virtual void OnViewportResize(uint32_t width, uint32_t height) override;

	private:
		Ref<Player> m_Player;
	};
}