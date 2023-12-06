#include "gepch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace GE {


	//////////////////////////////////////////////////////////
	/////////////////Vertex Buffer Below///////////////////////
	//////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* indices, uint32_t size)
	{
		GE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		//glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

	}
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		GE_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RendererID);
		//glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}
	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0,size, data);


	}
	void OpenGLVertexBuffer::Bind() const
	{
		GE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		GE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//////////////////////////////////////////////////////////
	/////////////////Index Buffer Below///////////////////////
	//////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}
	uint32_t OpenGLIndexBuffer::GetCount() const
	{
		return m_Count;
	}
	void OpenGLIndexBuffer::Bind() const
	{
		GE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		GE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}