#include "Game2DLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"


Game2D_Layer::Game2D_Layer() : Layer("Application 2D Renderer"), m_CameraController(1280 / 720, true)
{

}

void Game2D_Layer::OnAttach()
{
	using namespace GE;
	/////////////////////////////////////For square 
	m_SquareVA = VertexArray::Create();
	float squarevertices[5 * 4] = {
		-0.75f, -0.75f, 0.0f, 
		0.75f, -0.75f, 0.0f,
		-0.75f, 0.75f,0.0f,
		0.75f, 0.75f,0.0f
	};


	GE::Ref<VertexBuffer> squareVB = (VertexBuffer::Create(squarevertices, sizeof(squarevertices)));

	//Expand our layout so stride can get the correct offset
	BufferLayout sqlayout = {
		{ShaderDataType::Float3, "a_Position"}
	};

	squareVB->SetLayout(sqlayout);
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t indices2[6] = {
		0,2,1,3,2,1
	};
	//the count is 3 because we have 3 element in array
	GE::Ref<IndexBuffer> squareIB = IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t));
	m_SquareVA->AddIndexBuffer(squareIB);

	m_ColorShader = Shader::Create("assets/Shader/ColorShader.glsl");
}

void Game2D_Layer::OnDetach()
{
}

void Game2D_Layer::OnUpdate(GE::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	GE::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0 });
	GE::RenderCommand::Clear();




	GE::Renderer::BeginScene(m_CameraController.GetCamera());

	
	std::dynamic_pointer_cast<GE::OpenGLShader>(m_ColorShader)->Bind();
	std::dynamic_pointer_cast<GE::OpenGLShader>(m_ColorShader)->SetUniformFloat4("u_Color", m_SquareColor);
	
	m_ColorShader->Bind();
	GE::Renderer::Submit(m_ColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

	GE::Renderer::EndScene();

}

void Game2D_Layer::OnImGuiRender()
{
	ImGui::Begin("Setting");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Game2D_Layer::OnEvent(GE::Event& e)
{
	m_CameraController.OnEvent(e);
}
