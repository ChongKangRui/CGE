#include "gepch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"


namespace GE {
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			GE_CORE_ASSERT(false, "RendererAPI::None Not supported");
				return nullptr;
		}
			break;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
			break;
		}

		GE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			GE_CORE_ASSERT(false, "RendererAPI::None Not supported");
			return nullptr;
		}
			break;
		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(vertices, size);
			break;
		}

		GE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
