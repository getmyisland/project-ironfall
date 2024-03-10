#pragma once

#include <Engine/Core/Common.h>

#include <Engine/Core/Scene.h>
#include <Engine/Core/Window.h>
#include <Engine/Core/Timestep.h>
#include <Engine/Events/Event.h>
#include <Engine/Events/ApplicationEvent.h>

#include <mutex>

int main(int argc, char** argv);

namespace dyxide
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Application";
		Ref<Scene> DefaultScene;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void OnEvent(Event& e);

		void LoadScene(Ref<Scene> scene);

		Window& GetWindow() { return *m_Window; }

		void Close();

		static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }
		const Ref<Scene> GetScene() const { return m_Scene; }

		void SubmitToMainThread(const std::function<void()>& function);

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void ExecuteMainThreadQueue();

	private:
		ApplicationSpecification m_Specification;
		Scope<Window> m_Window;
		Ref<Scene> m_Scene;
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;

		std::vector<std::function<void()>> m_MainThreadQueue;
		std::mutex m_MainThreadQueueMutex;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}
