#pragma once

#include <Engine/Core/Application.h>

extern dyxide::Application* dyxide::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	auto app = dyxide::CreateApplication({ argc, argv });

	app->Run();

	delete app;
}