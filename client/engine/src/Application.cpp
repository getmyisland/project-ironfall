#include <Engine/Application.h>

#include <Shared/Assert.h>

namespace dyxide
{
    Application* Application::s_Instance = nullptr;

    Application::Application(ApplicationSpecification specification) : m_Specification(specification)
    {
        DYXIDE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
    }

    Application::~Application()
    {

    }
}