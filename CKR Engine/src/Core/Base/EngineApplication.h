#pragma once
#include "Core/Core.h"

#include "Window.h"
#include "Core/Event/ApplicationEvent.h"
//#include "Platform/Window/WindowsWindow.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Imgui/ImGuiLayer.h"
#include "LayerStack.h"


#include "Core/Renderer/Buffer.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/VertexArray.h"

#include "Core/Renderer/OrthographicCamera.h"
#include "TimeStep.h"
namespace GE {

	//! This will be the base application class to setup layer, create window etc.
	//! This is a singleton class which will only have one application throughout the whole program
	class GE_API Application
	{
	public:
		Application(const std::string& name = "C Game Engine");

		//will be inherited from game aplication
		virtual ~Application();
		//Run our application
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		void CloseApplication();


		ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;

	};
	// To be define in the client
	Application* CreateApplication();

}

