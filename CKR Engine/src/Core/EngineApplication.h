#pragma once
#include "Core.h"

#include "Window.h"
#include "Event/ApplicationEvent.h"
//#include "Platform/Window/WindowsWindow.h"
#include "Core/Event/MouseEvent.h"
#include "LayerStack.h"

namespace GE {


	class GE_API Application
	{
	public:
		Application();

		//will be inherited from game aplication
		virtual ~Application();
		//Run our application
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnButtonPressed(MouseButtonEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		LayerStack m_LayerStack;
	};
	// To be define in the client
	Application* CreateApplication();

}

