#include "Game2DLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
//Basic Timer Example
//#include <Chrono>
//
//template<typename fn>
//class Timer {
//public:
//	Timer(const char* name, fn&& function) : m_Name(name), m_Func(function), m_Stopped(false) {
//		m_StartTP = std::chrono::high_resolution_clock::now();
//	}
//
//	~Timer() {
//		if (!m_Stopped)
//			Stop();
//	}
//
//	void Stop() {
//		auto endTP = std::chrono::high_resolution_clock::now();
//		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTP).time_since_epoch().count();
//		auto End = std::chrono::time_point_cast<std::chrono::microseconds>(endTP).time_since_epoch().count();
//
//		float duration = End - start;
//		float ms = duration * 0.001f;
//
//		m_Stopped = true;
//
//		//Log_Info("{0} Duration: {1}",m_Name ,ms);
//		m_Func({ m_Name, ms });
//	}
//
//private:
//	const char* m_Name;
//	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTP;
//	bool m_Stopped;
//
//	fn m_Func;
//
//};
//#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResults profileResult) {m_ProfileResults.push_back(profileResult); })

Game2D_Layer::Game2D_Layer() : Layer("Application 2D Renderer"), m_CameraController(1280 / 720, true)
{

}

void Game2D_Layer::OnAttach()
{
	GE_PROFILE_FUNCTION();

	m_Texture2DExample = GE::Texture2D::Create("assets/Textures/TreeMat.png");
}

void Game2D_Layer::OnDetach()
{
	GE_PROFILE_FUNCTION();
}

void Game2D_Layer::OnUpdate(GE::TimeStep ts)
{
	GE_PROFILE_FUNCTION();
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


	{
		GE_PROFILE_SCOPE("Sandbox2D::OnRenderer");

		GE::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//White Diamond
		GE::Renderer2D::DrawQuad({ m_SquadPos.x,m_SquadPos.y + 1  }, { 1.0f, 1.0f }, 45.0f,{ 1.0f,1.0f,1.0f,1.0f });

		//Blue Diamond
		GE::Renderer2D::DrawQuad({  2,2 }, { 1.0f, 1.0f },0.0f, { 1.0f,0.3f,1.0f,1.0f });

		//Tree Texture
		GE::Renderer2D::DrawQuad({ m_SquadPos.x,m_SquadPos.y - 1.0f, 0.2f }, { 1.0f, 1.0f }, 1.0f,m_Texture2DExample, { 0.5,0.5,1.0,1.0 }, 1.0f);
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
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Game2D_Layer::OnEvent(GE::Event& e)
{
	m_CameraController.OnEvent(e);
}
