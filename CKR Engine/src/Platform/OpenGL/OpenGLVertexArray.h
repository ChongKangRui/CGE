#pragma once
#include "Core/Renderer/VertexArray.h"
namespace GE {
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddIndexBuffer(const Ref<IndexBuffer>& vertexBuffer) override;
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
	
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const {
			return m_VertexBuffer;
		};
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; };

	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		uint32_t m_RendererID;
	};
}

