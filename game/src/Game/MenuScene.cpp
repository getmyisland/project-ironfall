#include <Game/MenuScene.h>

#include <Engine/Core/Entity.h>
#include <Engine/Core/Components.h>

namespace dyxide
{
	void MenuScene::OnLoad()
	{
		auto ent = CreateEntity("title");
		ent.AddComponent<TextComponent>("Dyxide");
	}

	void MenuScene::OnUnload()
	{

	}

	void MenuScene::OnUpdateEntities(Timestep ts)
	{

	}

	void MenuScene::OnEvent(Event& e)
	{

	}
}