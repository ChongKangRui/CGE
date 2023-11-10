#include "gepch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace GE {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}