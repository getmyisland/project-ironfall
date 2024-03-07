#include <Engine/EntryPoint.h>

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
		specification.CommandLineArgs = args;

		return new Game(specification);
	}
}