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

		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& vertexBuffer) override;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
	
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const {
			return m_VertexBuffer;
		};
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; };

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

		uint32_t m_RendererID;
	};
}

