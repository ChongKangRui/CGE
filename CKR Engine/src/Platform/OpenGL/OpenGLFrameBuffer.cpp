#include "gepch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace GE {

	static const uint32_t s_MaxFrameBufferSize = 8192;

	namespace Utils {
		static bool IsDepthFormat(FramebufferTextureFormat format) {
			switch (format) {
			case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
			}
			return false;
		}

		static GLenum TextureTarget(bool multisample) {
			return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void BindTexture(bool multisample, uint32_t id) {
			glBindTexture(TextureTarget(multisample), id);
		}

		static void CreateTextures(bool multisample, uint32_t* outID, uint32_t count) {
			glCreateTextures(TextureTarget(multisample), count, outID);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, int index) {
			bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D,0 , format, width, height, 0, GL_RGBA,GL_UNSIGNED_BYTE, nullptr);
			
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,TextureTarget(multisampled), id, 0);


		}
		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentype, uint32_t width, uint32_t height) {
			bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else {
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentype, TextureTarget(multisampled), id, 0);


		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification) : m_Spec(specification)
	{

		for (auto texturespec : m_Spec.Attachments.TextureAttachments) {
			if (!Utils::IsDepthFormat(texturespec.TextureFormat))
				m_ColorAttachmentSpec.emplace_back(texturespec);
			else
				m_DepthAttahmentSpec = texturespec;

		}

		RecreateBuffer();
	}
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}
	void OpenGLFramebuffer::RecreateBuffer()
	{
		if (m_RendererID) {

			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Spec.Samples > 1;

		if (m_ColorAttachmentSpec.size()) {
			m_ColorAttachments.resize(m_ColorAttachmentSpec.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());
			//Attachments
			for (size_t i = 0; i < m_ColorAttachmentSpec.size(); i++) {
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpec[i].TextureFormat) {
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Spec.Samples, GL_RGBA8, m_Spec.Width, m_Spec.Height, i);
					break;
					
				}

			}
		}
		if (m_DepthAttahmentSpec.TextureFormat != FramebufferTextureFormat::None) {
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthAttahmentSpec.TextureFormat) {
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Spec.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Spec.Width, m_Spec.Height);
				break;
			}
		}

		if (m_ColorAttachments.size() > 1) {
			GE_CORE_ASSERT(m_ColorAttachments.size() <= 4, " ");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		
		}
		else if (m_ColorAttachments.empty()) {
			// only depth-pass
			glDrawBuffer(GL_NONE);
		}
			
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		//glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Spec.Width, m_Spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		////////Depth buffer
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		//glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Spec.Width, m_Spec.Height);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		GE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Spec.Width, m_Spec.Height);
	}
	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{

		if (width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize) {
			GELog_Error("Attempted to resize framebuffer to {0}, {1} ", width, height);
			return;
		}


		m_Spec.Width = width;
		m_Spec.Height = height;

		RecreateBuffer();
	}
}