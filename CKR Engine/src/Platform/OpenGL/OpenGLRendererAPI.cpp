#include "gepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>
namespace GE {



	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

	}

	void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);

	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const GE::Ref<VertexArray>& vertexArray, uint32_t IndexCount)
	{
		uint32_t count = IndexCount ? vertexArray->GetIndexBuffer()->GetCount() : IndexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		
	}

}
