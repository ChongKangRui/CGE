#pragma once
#include "Core/Renderer/FrameBuffer.h"


namespace GE {
	class OpenGLFramebuffer : public FrameBuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual const FramebufferSpecification& GetSpecification() const override {
			return m_Spec;
		}

		/*virtual uint32_t GetColorAttachmentRendererID() const override {
			return m_ColorAttachment;
		}*/
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override {
			GE_CORE_ASSERT(index < m_ColorAttachments.size(), " ");
			return m_ColorAttachments[index];
		}
		void RecreateBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

	private:
		uint32_t m_RendererID =0, /*m_ColorAttachment = 0, */m_DepthAttachment = 0;
		FramebufferSpecification m_Spec;

		std::vector<FrambufferTextureSpecification> m_ColorAttachmentSpec;
		FrambufferTextureSpecification m_DepthAttahmentSpec = FramebufferTextureFormat::None;
	
		std::vector<uint32_t> m_ColorAttachments;
		
	
	};
}

