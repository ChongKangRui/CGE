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

	private:
		uint32_t m_RendererID, m_ColorAttachment, m_DepthAttachment;
		FrameBufferSpecification m_Spec;

	};
}

