#include "gepch.h"
#include "EngineApplication.h"


#include "Core/Event/Event.h"


#include "Input.h"

#include "Core/Renderer/Renderer.h"
#include "KeyCode.h"
#include "MouseCode.h"

#include <GLFW/glfw3.h>

namespace GE {


#define Bind_Event_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	//Create window, set window event callback, everytime input trigger, callbackevent will trigger
	GE::Application::Application(const std::string& name)
	{
		GE_PROFILE_FUNCTION();
		//GE_CORE_ASSERT(s_Instance, "Application already exists!");
		s_Instance = this;
		//GE_CORE_ASSERT(!s_Instance, "Application aldy exist");

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(Bind_Event_FN(OnEvent));
		
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	GE::Application::~Application()
	{
	}

	//While loop for window, constantly update layer
	void Application::Run()
	{

		GE_PROFILE_FUNCTION();


		if (!m_Window) {
			GELog_Fatal("No Window Reference created from application");
			return;
		}
		while (m_Running) {

			GE_PROFILE_SCOPE("Engine::RunLoop()");

			float time = (float)glfwGetTime(); ///platform::GetTime()
			TimeStep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				//Layer Update in layerstack
				{
					GE_PROFILE_SCOPE("LayerStack::Layer::OnUpdate()");
					for (Layer* layer : m_LayerStack) {
						layer->OnUpdate(timestep);
					}
				}

				//to render every layer
				m_ImGuiLayer->Begin();
				{
					GE_PROFILE_SCOPE("LayerStack::Layer::OnImGuiRender()");
					for (Layer* layer : m_LayerStack) {
						layer->OnImGuiRender();
					}
				}
				m_ImGuiLayer->End();

			}

			


			//Window Update
			m_Window->OnUpdate();


		}

	}

	//Event callback that bind to oneventcallback
	void Application::OnEvent(Event& e)
	{
		GE_PROFILE_FUNCTION();


		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(Bind_Event_FN(OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(Bind_Event_FN(OnWindowResize));


		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}


	}

	void Application::PushLayer(Layer* layer)
	{
		GE_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		GE_PROFILE_FUNCTION();


		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}



	void Application::CloseApplication()
	{
		m_Running = false;
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		GE_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	

	
}