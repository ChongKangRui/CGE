#include "gepch.h"
#include "OpenGLContext.h"

#include <glfw/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

#include "Core/Log.h"

namespace GE {
	OpenGLContext::OpenGLContext(GLFWwindow* window): m_WindowHandle(window)
	{

	}
	
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		//GE_CORE_ASSERT(status, "Failed To Initialize Glad");
		GELog_Info("Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		GELog_Info("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		GELog_Info("Version: {0}", (const char*)glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		//Test only
	
		glfwSwapBuffers(m_WindowHandle);
	}
}