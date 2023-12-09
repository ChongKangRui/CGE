#include "MapLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

static const uint32_t s_MapWidth = 36;
static const char* s_MapTiles =
"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT"
"T0000000000000000000000000000000000T"
"T0000001111111111111111111100000000T"
"T0000000000011111111111111110000000T"
"T0000001111111111111111111111110000T"
"T0000000001111111111111111000000000T"
"T000000000000000000000TTTTTTTTT0000T"
"T0000111111111111111111111111111100T"
"T0000011111111111111111111111110000T"
"T0000TTTTTTTTTT00000000000000000000T"
"T0000011111111111111111111111000000T"
"T0000000000000000000000000000000000T"
"T0000000000000000000000000000000000T"
"T0000000000000000000000000000000000T"
"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT"
"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT";




Map_Layer::Map_Layer() : Layer("Application 2D Renderer"), m_CameraController(1280 / 720, true), m_ParticleSystem(10000)
{

}

void Map_Layer::OnAttach()
{
	GE_PROFILE_FUNCTION();

	m_Texture2DExample = GE::Texture2D::Create("assets/Textures/TreeMat.png");
	m_SpriteSheet = GE::Texture2D::Create("assets/Textures/RPGpack_sheet.png");
	//m_Subtexture = GE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7,6 }, { 128/2,128/2 });
	s_TextureMap['T'] = GE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1,11 }, { 128 / 2,128 / 2 }, {1,1});

	m_TextureTree = GE::SubTexture2D::CreateFromCoords(m_SpriteSheet, {2,1}, {128 / 2,128 / 2}, {1,2});

	s_TextureMap['1'] = GE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11,11 }, { 128 / 2,128 / 2 });
	s_TextureMap['0'] = GE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6,11 }, { 128 / 2,128 / 2 });
	//s_TextureMap['T'] = GE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2,1 }, { 128 / 2,128 / 2 }, { 1,2 });

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	//ParticleInitialize
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };


	m_CameraController.SetZoomLevel(3);
}

void Map_Layer::OnDetach()
{
	GE_PROFILE_FUNCTION();
}

void Map_Layer::OnUpdate(GE::TimeStep ts)
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

		//GE::Renderer2D::DrawQuad({ m_SquadPos.x,m_SquadPos.y - 3.0f, 0.2f }, { 1.0f, 1.0f }, 0.0f, m_SpriteSheet);
		/*GE::Renderer2D::DrawQuad({ m_SquadPos.x,m_SquadPos.y - 3.0f, 0.2f }, { 1.0f, 2.0f }, 0.0f, m_TextureStairs);
		GE::Renderer2D::DrawQuad({ m_SquadPos.x,m_SquadPos.y + 1.0f, 0.2f }, { 1.0f, 2.0f }, 0.0f, m_TextureTree);
		GE::Renderer2D::DrawQuad({ m_SquadPos.x,m_SquadPos.y + 3.0f, 0.2f }, { 1.0f, 2.0f }, 0.0f, m_TextureBarrel);*/

		for (uint32_t y = 0; y < m_MapHeight; y++) {
			for (uint32_t x = 0; x < m_MapWidth; x++) {
			
				char tileType = s_MapTiles[x + y * m_MapWidth];
				GE::Ref<GE::SubTexture2D> texture;


				if (s_TextureMap.find(tileType) != s_TextureMap.end()) {
					texture = s_TextureMap[tileType];
				}
				else
					texture = m_TextureTree;

				//if (tileType == 'T') {
				//	//GE::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f,y - m_MapHeight / 2.0f + 1, 0.4f }, { 1.0f, 2.0f }, 0.0f, texture);

				//	auto gtexture = s_TextureMap['0'];
				//	GE::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f,y - m_MapHeight / 2.0f + 1, 0.2f }, { 1.0f, 1.0f }, 0.0f, gtexture);
				//}
				//else
					GE::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f,y - m_MapHeight / 2.0f, 0.2f }, { 1.0f, 1.0f }, 0.0f, texture);

			}

		}

		GE::Renderer2D::EndScene();



		
	}
	

	//Todo Shader::SetMAt4, Shader::SetFloat4 etc;
	//GE::Renderer::Submit(m_ColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
	//std::dynamic_pointer_cast<GE::OpenGLShader>(m_ColorShader)->Bind();
	//std::dynamic_pointer_cast<GE::OpenGLShader>(m_ColorShader)->SetUniformFloat4("u_Color", m_SquareColor);


}

void Map_Layer::OnImGuiRender()
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

void Map_Layer::OnEvent(GE::Event& e)
{
	m_CameraController.OnEvent(e);
}
