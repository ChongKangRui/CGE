#include "Game2DLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Game2D_Layer::Game2D_Layer() : Layer("Application 2D Renderer"), m_CameraController(1280 / 720, true), m_ParticleSystem(10000)
{

}

void Game2D_Layer::OnAttach()
{
	GE_PROFILE_FUNCTION();

	m_Texture2DExample = GE::Texture2D::Create("assets/Textures/TreeMat.png");

	//ParticleInitialize
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void Game2D_Layer::OnDetach()
{
	GE_PROFILE_FUNCTION();
}

void Game2D_Layer::OnUpdate(GE::TimeStep ts)
{
	GE_PROFILE_FUNCTION();
	m_CameraController.OnUpdate(ts);
	
	GE::Renderer2D::ResetStats();
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



	if (GE::Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		auto [x, y] = GE::Input::GetMousePos();
		auto width = GE::Application::Get().GetWindow().GetWidth();
		auto height = GE::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	{
		GE_PROFILE_SCOPE("Sandbox2D::OnRenderer");

		GE::Renderer2D::BeginScene(m_CameraController.GetCamera());

		//Blue Diamond
		GE::Renderer2D::DrawQuad({ m_SquadPos.y + 7,m_SquadPos.y + 2 }, { 1.0f, 2.0f }, 0.0f, { 1.0f,0.3f,1.0f,1.0f });

		//White Diamond
		GE::Renderer2D::DrawQuad({ m_SquadPos.x + 6,m_SquadPos.y + 5}, { 1.0f, 1.0f }, 0.0f, { 1.0f,1.0f,0.0f,1.0f });

		//Tree Texture
		GE::Renderer2D::DrawQuad({ m_SquadPos.x,m_SquadPos.y - 1.0f, 0.2f }, { 1.0f, 1.0f }, 0.0f, m_Texture2DExample,{ 0.5,0.5,1.0,1.0 });

		//	GE::Renderer2D::DrawQuad({ 2,1 }, { 0.45f, 0.45f }, 0.0f, { 0.5,0.5,1.0,1.0 });

		GE::Renderer2D::EndScene();

		
		GE::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {

				glm::vec4 ccccc = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };

				GE::Renderer2D::DrawQuad({ x + 10, y + 10 }, { 0.45f, 0.45f },0.0f,  { 0.5,0.5,1.0,1.0 });

			}
		}
		GE::Renderer2D::EndScene();

		
	}
	

	//Todo Shader::SetMAt4, Shader::SetFloat4 etc;
	//GE::Renderer::Submit(m_ColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
	//std::dynamic_pointer_cast<GE::OpenGLShader>(m_ColorShader)->Bind();
	//std::dynamic_pointer_cast<GE::OpenGLShader>(m_ColorShader)->SetUniformFloat4("u_Color", m_SquareColor);


}

void Game2D_Layer::OnImGuiRender()
{
	GE_PROFILE_FUNCTION();

	ImGui::Begin("Setting");

	
	auto stat = GE::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stat: ");
	ImGui::Text("Draw Call: %d", stat.DrawCall);
	ImGui::Text("Quad Count: %d", stat.QuadCount);
	ImGui::Text("Vertices: %d", stat.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stat.GetTotalIndexCount());



	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Game2D_Layer::OnEvent(GE::Event& e)
{
	m_CameraController.OnEvent(e);
}
