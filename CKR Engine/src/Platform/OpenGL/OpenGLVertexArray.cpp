#include "gepch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>



namespace GE {

	static GLenum ShaderDataTypeToOpenGlBaseType(ShaderDataType type) {

		switch (type) {
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat3: return GL_FLOAT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2: return GL_INT;
		case ShaderDataType::Int3: return GL_INT;
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;

		}

		GE_CORE_ASSERT(false, "UnknownShaderType");
		return 0;
	};


	OpenGLVertexArray::OpenGLVertexArray()
	{
		GE_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
	}
	void OpenGLVertexArray::Bind() const
	{
		GE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		GE_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		GE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;

	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		// Profile the function using a profiler (for profiling performance)
		GE_PROFILE_FUNCTION();

		// Bind the vertex array object
		glBindVertexArray(m_RendererID);

		// Bind the given vertex buffer
		vertexBuffer->Bind();

		// Ensure that the vertex buffer has a layout
		GE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

		// Initialize index for vertex attribute arrays
		uint32_t index = 0;

		// Iterate over each element in the vertex buffer layout
		for (const auto& element : vertexBuffer->GetLayout()) {
			// Enable the vertex attribute array at the specified index
			glEnableVertexAttribArray(index);

			// Set up the vertex attribute pointer
			glVertexAttribPointer(
				index,                           // Index of the generic vertex attribute
				element.GetComponentCount(),    // Number of components per attribute
				ShaderDataTypeToOpenGlBaseType(element.Type),  // Data type of each component
				element.Normalized ? GL_TRUE : GL_FALSE,        // Whether data should be normalized
				vertexBuffer->GetLayout().GetStride(),           // Stride between consecutive generic vertex attributes
				(const void*)element.Offset       // Offset of the first component of the first generic vertex attribute in the array
			);

			// Increment the index for the next vertex attribute array
			index++;

			// Log information about the stride of the vertex layout
			Log_Info("{0}", vertexBuffer->GetLayout().GetStride());
		}

		// Store the vertex buffer in the list of vertex buffers associated with the vertex array
		m_VertexBuffers.push_back(vertexBuffer);
	}

}