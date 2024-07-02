#include <dxpch.h>
#include <Engine/Core/Application.h>

#include <Engine/Core/Assert.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/Time.h>
#include <Engine/Events/Event.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Renderer/RenderCommand.h>
#include <Engine/Renderer/RendererUI.h>

namespace dyxide
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		DYXIDE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		for (int i = 0; i < m_Specification.CommandLineArgs.Count; i++)
		{
			// Check if the game was started in debug mode
			if (std::strcmp(m_Specification.CommandLineArgs[i], "-d") == 0 || std::strcmp(m_Specification.CommandLineArgs[i], "--debug") == 0)
			{
				m_Debug = true;
				break;
			}
		}

		m_Window = Window::Create(WindowProps(m_Specification.Name));
		m_Window->SetEventCallback(DYXIDE_BIND_EVENT_FN(Application::OnEvent));

		Input::Init();

		RenderCommand::Init();
		Renderer::Init();
		RendererUI::Init();
	}

	Application::~Application()
	{
		Renderer::Shutdown();
		RendererUI::Shutdown();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::SubmitToMainThread(const std::function<void()>& function)
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		m_MainThreadQueue.emplace_back(function);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(DYXIDE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(DYXIDE_BIND_EVENT_FN(Application::OnWindowResize));

		if (!e.Handled && m_Scene)
			m_Scene->OnEvent(e);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = Time::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			ExecuteMainThreadQueue();

			if (!m_Minimized)
			{
				if (m_Scene)
					m_Scene->OnUpdate(timestep);
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;

		RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		RendererUI::OnWindowResize(e.GetWidth(), e.GetHeight());

		m_Scene->OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::ExecuteMainThreadQueue()
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		for (auto& func : m_MainThreadQueue)
			func();

		m_MainThreadQueue.clear();
	}
}
