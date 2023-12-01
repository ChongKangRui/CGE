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
	m_Texture2DExample = GE::Texture2D::Create("assets/Textures/TreeMat.png");
}

void Game2D_Layer::OnDetach()
{
	
}

void Game2D_Layer::OnUpdate(GE::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	GE::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0 });
	GE::RenderCommand::Clear();


	//Temp for testing only
	if (GE::Input::IsKeyPressed(KEY_I))
		m_SquadPos.y += 1 * ts;
	else if (GE::Input::IsKeyPressed(KEY_K))
		m_SquadPos.y -= 1 * ts ;
	if (GE::Input::IsKeyPressed(KEY_J))
		m_SquadPos.x -= 1 * ts ;
	else if (GE::Input::IsKeyPressed(KEY_L))
		m_SquadPos.x += 1 * ts ;

	if (GE::Input::IsKeyPressed(KEY_U))
		m_Rotation += 1 * ts;
	else if (GE::Input::IsKeyPressed(KEY_O))
		m_Rotation -= 1 * ts;


	GE::Renderer2D::BeginScene(m_CameraController.GetCamera());
	GE::Renderer2D::DrawQuad({ m_SquadPos.x,m_SquadPos.y}, { 1.0f, 1.0f }, { 1.0f,1.0f,1.0f,1.0f });
	GE::Renderer2D::DrawQuad({ m_SquadPos.x,m_SquadPos.y + 1.0f, -0.1f }, { 1.0f, 1.0f }, m_Texture2DExample);
	GE::Renderer2D::EndScene();

	//Todo Shader::SetMAt4, Shader::SetFloat4 etc;
	//GE::Renderer::Submit(m_ColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
	//std::dynamic_pointer_cast<GE::OpenGLShader>(m_ColorShader)->Bind();
	//std::dynamic_pointer_cast<GE::OpenGLShader>(m_ColorShader)->SetUniformFloat4("u_Color", m_SquareColor);


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
