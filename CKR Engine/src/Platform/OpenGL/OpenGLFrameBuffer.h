#pragma once
#include "Core/Renderer/FrameBuffer.h"


namespace GE {
	class OpenGLFramebuffer : public FrameBuffer
	{
	public:
		OpenGLFramebuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual const FrameBufferSpecification& GetSpecification() const override {
			return m_Spec;
		}

		virtual uint32_t GetColorAttachmentRendererID() const override {
			return m_ColorAttachment;
		}

		void Resize();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

	private:
		uint32_t m_RendererID =0, m_ColorAttachment = 0, m_DepthAttachment = 0;
		FrameBufferSpecification m_Spec;

	};
}

