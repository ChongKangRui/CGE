#pragma once
#include "Core/Renderer/GraphicContext.h"

struct GLFWwindow;

namespace GE {
	class OpenGLContext : public GraphicContext
	{
	public:
		OpenGLContext(GLFWwindow* window);


		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}

