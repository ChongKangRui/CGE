#include "gepch.h"
#include "OpenGLContext.h"

#include <glfw/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

#include "Core/Base/Log.h"

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

#ifdef GE_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		 
		GE_CORE_ASSERT(versionMajor > 4 || (versionMajor >= 4 && versionMinor >= 5), "CGE require at least 4.5");

#endif

	}
	void OpenGLContext::SwapBuffers()
	{
		//Test only
	
		glfwSwapBuffers(m_WindowHandle);
	}
}