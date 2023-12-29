#pragma once

#include "Core/Core.h"



namespace GE {

	enum class FramebufferTextureFormat {
		None = 0,

		// Color
		RGBA8,

		// Depth/stencil
		DEPTH24STENCIL8,

		//Defaults
		Depth = DEPTH24STENCIL8
	};

	//Represent each individual texture that add to framebuffer
	struct FrambufferTextureSpecification {
		FrambufferTextureSpecification() = default;
		FrambufferTextureSpecification(FramebufferTextureFormat format) : TextureFormat(format) {}
		
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// Todo: filtering/wrap
	
	};

	//Collective of framebuffer attachment
	struct FramebufferAttachmentSpecification {

		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(const std::initializer_list<FrambufferTextureSpecification> attachment) : TextureAttachments(attachment) {

		}
	
		std::vector<FrambufferTextureSpecification> TextureAttachments;
	};

	struct FramebufferSpecification {
		uint32_t Width, Height; 
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
		//  meaning in opengl glBindFrameBuffer(0);
	};




	class FrameBuffer
	{
	public:

		static Ref<FrameBuffer> Create(const FramebufferSpecification& spec);

		virtual ~FrameBuffer() = default;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		//virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		

	};

}
