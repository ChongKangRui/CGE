#pragma once
#include "Core/Renderer/Buffer.h"
#include "gepch.h"

namespace GE {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* indices, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override{ return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override {
			m_Layout = layout;
		}

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};


	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		virtual uint32_t GetCount() const;
		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint32_t m_Count;
		uint32_t m_RendererID;
	};
}
