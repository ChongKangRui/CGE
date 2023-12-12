#pragma once

#include "Core/Core.h"



namespace GE {

	struct FrameBufferSpecification {
		uint32_t width, height; 

		uint32_t Samples = 1;

		bool SwapChainTarget = false;
		//  meaning in opengl glBindFrameBuffer(0);
	};




	class FrameBuffer
	{
	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

		virtual const FrameBufferSpecification& GetSpecification() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		

	};

}
