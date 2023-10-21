#include "gepch.h"
#include "WindowsWindow.h"
#include "Core/Log.h"

#include "Core/Event/MouseEvent.h"
#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/KeyEvent.h"

#include <glad/glad.h>


namespace GE {


	static bool s_GLFWInitialized = false;
	static void GLFWErrorCallback(int error, const char* description) {
		GELog_Error("GLFW Error {0}: {1}", error, description);
	}
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}



	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		GELog_Info("Window Initialization");
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
	}
	void WindowsWindow::OnUpdate()
	{
		if (m_Window) {
			glfwPollEvents();
			glfwSwapBuffers(m_Window);
		}
		else {
			GELog_Fatal("No Window Reference");
		}
	}
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}
	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		GELog_Info("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
		if (!s_GLFWInitialized) {

			int success = glfwInit();
			//GELog_Asse
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height,
			m_Data.Title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		//GE_CORE_ASSERT(status, "Failed To Initialize Glad");

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		

		//All the lambda retrieve the data from the pointer that set at glfwSetWindowUserPointer
		//everytime glfw detect the input that it set below, 
		// it will call the binded function(which is onevent in engine application) data.EventCallback(event); 
		// and once onevent being call, all the layer in layer stack 
		// onevent will be called as well which is where we bind the event into the dispatcher

		//Set GLFW callbacks
		auto Resizelambda = ([](GLFWwindow* window, int Width, int Height) {
			//Get Window data from the above setwindowuserpointer(void pointer)
			//kinda like retrieve it from the second arguement
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);


			data.Width = Width;
			data.Height = Height;
			WindowResizeEvent event(Width, Height);
			data.EventCallback(event);


			});
		glfwSetWindowSizeCallback(m_Window, Resizelambda);

		auto Closelambda = ([](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
			});
		glfwSetWindowCloseCallback(m_Window, Closelambda);

		auto Keylambda = ([](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			

			switch (action) {
			case GLFW_PRESS: {
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				KeyReleasedEvent event(key);
				data.EventCallback(event);

				break;
			}
			case GLFW_REPEAT: {
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
			});
		glfwSetKeyCallback(m_Window, Keylambda);

		auto KeyCharLambda = ([](GLFWwindow* window, unsigned int keycode) {
			
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypeEvent event(keycode);
			data.EventCallback(event);
			});
		glfwSetCharCallback(m_Window, KeyCharLambda);

		//MouseButton
		auto MouseButtonlambda = ([](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}

			}
			});
		glfwSetMouseButtonCallback(m_Window, MouseButtonlambda);

		//Mouse scrolled
		auto MouseScrolledlambda = ([](GLFWwindow* window, double xoffset, double yoffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);


			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.EventCallback(event);
			});
		glfwSetScrollCallback(m_Window, MouseScrolledlambda);

		//Mouse pos
		auto MousePoslambda = ([](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);


			MouseMovedEvent event((float)x, (float)y);
			data.EventCallback(event);
			});
		glfwSetCursorPosCallback(m_Window, MousePoslambda);

		

	}
	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}



}

