#include "gepch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h" 


namespace GE {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4 transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_ModelTransform", transform);

		va->Bind();
		RenderCommand::DrawIndexed(va);
	}

	void Renderer2D::Init()
	{
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4 transform)
	{
	}

}

