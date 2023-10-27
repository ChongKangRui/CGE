#pragma once
#include "Core.h"

#include "Window.h"
#include "Event/ApplicationEvent.h"
//#include "Platform/Window/WindowsWindow.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Imgui/ImGuiLayer.h"
#include "LayerStack.h"

namespace GE {

	//! This will be the base application class to setup layer, create window etc.
	//! This is a singleton class which will only have one application throughout the whole program
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

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnButtonPressed(MouseButtonEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;

		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
	};
	// To be define in the client
	Application* CreateApplication();

}

