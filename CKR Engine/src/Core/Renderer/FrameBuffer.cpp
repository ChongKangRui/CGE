#include "gepch.h"
#include "FrameBuffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace GE {
	Ref<FrameBuffer> FrameBuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			GE_CORE_ASSERT(false, "RendererAPI::None Not supported");
			return nullptr;
		}
								   break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);
			break;
		}

		GE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;

	}
}