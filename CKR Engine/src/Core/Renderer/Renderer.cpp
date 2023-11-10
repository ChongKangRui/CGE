#include "gepch.h"
#include "Renderer.h"

namespace GE {

	

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& va)
	{
		va->Bind();
		RenderCommand::DrawIndexed(va);
	}

}

