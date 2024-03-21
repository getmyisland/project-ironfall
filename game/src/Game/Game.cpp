#include <Engine/Core/EntryPoint.h>

#include <Game/MenuScene.h>
#include <Game/PlayScene.h>

namespace dyxide
{
	class Game : public Application
	{
	public:
		Game(const ApplicationSpecification& specification) : Application(specification)
		{
			LoadScene<PlayScene>();
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification specification;
		specification.Name = "Dyxide";
		specification.CommandLineArgs = args;

		return new Game(specification);
	}
}