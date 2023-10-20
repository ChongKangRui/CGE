#include "gepch.h"
#include "EngineApplication.h"


#include "Event/Event.h"
#include "Log.h"
#include <glad/glad.h>

namespace GE {


#define Bind_Event_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	//Create window, set window event callback, everytime input trigger, callbackevent will trigger
	GE::Application::Application()
	{
		//GE_CORE_ASSERT(s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(Bind_Event_FN(OnEvent));
		
	}

	GE::Application::~Application()
	{
	}

	//While loop for window, constantly update layer
	void Application::Run()
	{
		if (!m_Window) {
			GELog_Fatal("No Window Reference created from application");
		}
		while (m_Running) {
			glClearColor(1, 0.5, 0.5, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			//Layer Update in layerstack
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			//Window Update
			m_Window->OnUpdate();



		}
		
	}

	//Event callback that bind to oneventcallback
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e); 
	
		//dispatcher.Dispatch<MouseButtonPressedEvent>(Bind_Event_FN(OnButtonPressed));
		//dispatcher.Dispatch<WindowCloseEvent>(Bind_Event_FN(OnWindowClosed));
		
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
		
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}



	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnButtonPressed(MouseButtonEvent& e)
	{
		GELog_Info("Button press liao");
		return true;
	}

}