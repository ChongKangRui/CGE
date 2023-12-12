#include "gepch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"


namespace GE {
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			GE_CORE_ASSERT(false, "RendererAPI::None Not supported");
				return nullptr;
		}
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
			break;
		}

		GE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			GE_CORE_ASSERT(false, "RendererAPI::None Not supported");
			return nullptr;
		}
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
			break;
		}

		GE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* vertices, uint32_t count) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			GE_CORE_ASSERT(false, "RendererAPI::None Not supported");
			return nullptr;
		}
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(vertices, count);
			break;
		}

		GE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
