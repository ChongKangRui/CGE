#include "gepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace GE {
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			GE_CORE_ASSERT(false, "RendererAPI::None Not supported");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
			return  std::make_shared<OpenGLVertexArray>();
			
		}

		GE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}