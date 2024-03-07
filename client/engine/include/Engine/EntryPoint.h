#ifndef DYXIDE_ENTRY_POINT_H
#define DYXIDE_ENTRY_POINT_H

#include <Engine/Application.h>

namespace dyxide
{
	extern Application* CreateApplication(ApplicationCommandLineArgs args);
}

int main(int argc, char** argv)
{
	auto app = dyxide::CreateApplication({ argc, argv });

	// run app

	delete app;
}

#endif