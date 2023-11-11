#include "gepch.h"
#include "EngineApplication.h"


#include "Event/Event.h"
#include "Log.h"
#include "Input.h"

#include "Renderer/Renderer.h"
#include "KeyCode.h"
#include "MouseCode.h"


namespace GE {


#define Bind_Event_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	//Create window, set window event callback, everytime input trigger, callbackevent will trigger
	GE::Application::Application() : m_Camera(-3.0f, 3.0f, -3.0f, 3.0f)
	{
		//GE_CORE_ASSERT(s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(Bind_Event_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

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
			//Layer Update in layerstack
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			//to render every layer
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnRender();
			}
			m_ImGuiLayer->End();


			//Window Update
			m_Window->OnUpdate();


		}

	}

	//Event callback that bind to oneventcallback
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(Bind_Event_FN(OnWindowClosed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(Bind_Event_FN(OnButtonPressed));
		dispatcher.Dispatch<KeyPressedEvent>(Bind_Event_FN(OnKeyPressed));
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
		switch (e.GetMouseButton()) {
		case MOUSE_BUTTON_LEFT:
			m_Camera.SetRotation(m_Camera.GetRotation() + 3);
			break;
		case MOUSE_BUTTON_RIGHT:
			m_Camera.SetRotation(m_Camera.GetRotation() - 3);
			break;

		}
		return true;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode()) {
		case KEY_A: {

			m_Camera.SetPosition({ m_Camera.GetPosition().x + 0.1f,  m_Camera.GetPosition().y ,  m_Camera.GetPosition().z });
		}
				  break;
		case KEY_D: {

			m_Camera.SetPosition({ m_Camera.GetPosition().x - 0.1f,  m_Camera.GetPosition().y ,  m_Camera.GetPosition().z });
		}
				  break;
		case KEY_S: {

			m_Camera.SetPosition({ m_Camera.GetPosition().x ,  m_Camera.GetPosition().y - 0.1f,  m_Camera.GetPosition().z });
		}
				  break;
		case KEY_W: {

			m_Camera.SetPosition({ m_Camera.GetPosition().x ,  m_Camera.GetPosition().y + 0.1f,  m_Camera.GetPosition().z });
		}
				  break;
		}



		return true;
	}

}