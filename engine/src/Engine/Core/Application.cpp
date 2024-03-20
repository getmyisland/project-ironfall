#include <dxpch.h>
#include <Engine/Core/Application.h>

#include <Engine/Core/Assert.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/Time.h>
#include <Engine/Events/Event.h>
#include <Engine/Renderer/Renderer.h>

namespace dyxide
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		DYXIDE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(m_Specification.Name));
		m_Window->SetEventCallback(DYXIDE_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
	}

	Application::~Application()
	{
		Renderer::Shutdown();
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
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

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
