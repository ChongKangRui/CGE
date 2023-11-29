#include "gepch.h"
//#include "Log.h"
//#include "Window.h"
//
//
//namespace GEngine {
//	Window::~Window()
//	{
//	}
//	static bool s_GLFWInitialized = false;
//
//	Window* Window::Create(const WindowProps& props)
//	{
//		return new Window(props);
//	}
//
//	Window::Window(const WindowProps& props)
//	{
//		GELog_Info("??????sdasda");
//		Init(props);
//	}
//	
//	void Window::OnUpdate()
//	{
//		if (m_Window) {
//			glfwPollEvents();
//			glfwSwapBuffers(m_Window);
//		}
//		else {
//			GELog_Fatal("No Window Reference");
//		}
//	}
//	void Window::SetEventCallback(const EventCallbackFn& callback)
//	{
//
//	}
//	void Window::SetVSync(bool enabled)
//	{
//		if (enabled)
//			glfwSwapInterval(1);
//		else
//			glfwSwapInterval(0);
//
//		m_Data.VSync = enabled;
//	}
//	bool Window::IsVSync() const
//	{
//		return m_Data.VSync;
//	}
//
//	void Window::Init(const WindowProps& props)
//	{
//		m_Data.Title = props.Title;
//		m_Data.Width = props.Width;
//		m_Data.Height = props.Height;
//
//		GELog_Info("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
//		if (!s_GLFWInitialized) {
//
//			int success = glfwInit();
//			//GELog_Asse
//
//			s_GLFWInitialized = true;
//		}
//
//		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height,
//			m_Data.Title.c_str(), nullptr, nullptr);
//
//		glfwMakeContextCurrent(m_Window);
//		glfwSetWindowUserPointer(m_Window, &m_Data);
//		SetVSync(true);
//
//
//	}
//	void Window::ShutDown()
//	{
//		glfwDestroyWindow(m_Window);
//	}
//}
