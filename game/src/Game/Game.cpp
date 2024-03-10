#include <Engine/Core/EntryPoint.h>

#include <Game/MenuScene.h>

namespace dyxide
{
	class Game : public Application
	{
	public:
		Game(const ApplicationSpecification& specification) : Application(specification)
		{

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification specification;
		specification.Name = "Dyxide";
		specification.DefaultScene = CreateRef<MenuScene>();
		specification.CommandLineArgs = args;

		return new Game(specification);
	}
}