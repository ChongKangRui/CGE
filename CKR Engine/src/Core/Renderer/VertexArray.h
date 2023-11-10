#pragma once
#include "Buffer.h"
#include <memory>

namespace GE {
	class VertexArray {
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& vertexBuffer)  = 0;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)  = 0;


		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;



		static VertexArray* Create();
	};
}
