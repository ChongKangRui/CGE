#pragma once
#include "Buffer.h"
#include <memory>

namespace GE {
	class VertexArray {
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddIndexBuffer(const Ref<IndexBuffer>& vertexBuffer)  = 0;
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)  = 0;


		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;



		static VertexArray* Create();
	};
}

