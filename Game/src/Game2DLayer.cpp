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

	m_TextureTree = GE::Texture2D::Create("assets/Textures/TreeMat.png");
	m_SpriteSheet = GE::Texture2D::Create("assets/Textures/RPGpack_sheet.png");
	//m_Subtexture = GE::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7,6 }, { 128/2,128/2 });

	//ParticleInitialize
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };


	GE::FrameBufferSpecification spec;
	spec.width = 1280;
	spec.height = 720;
	
	m_FrameBuffer = GE::FrameBuffer::Create(spec);


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
	{
		GE_PROFILE_SCOPE("Renderer Prep");

		m_FrameBuffer->Bind();
		GE::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0 });
		GE::RenderCommand::Clear();
	}


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
		GE::Renderer2D::DrawQuad({ m_SquadPos.x,m_SquadPos.y - 1.0f, 0.2f }, { 1.0f, 1.0f }, 0.0f, m_TextureTree,{ 0.5,0.5,1.0,1.0 });

		
		GE::Renderer2D::EndScene();

		
		GE::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {

				glm::vec4 TileColor = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };

				GE::Renderer2D::DrawQuad({ x + 10, y + 10 }, { 0.45f, 0.45f },0.0f,  { TileColor });

			}
		}
		GE::Renderer2D::EndScene();
		m_FrameBuffer->Unbind();



		
	}
	

	//Todo Shader::SetMAt4, Shader::SetFloat4 etc;
	//GE::Renderer::Submit(m_ColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
	//std::dynamic_pointer_cast<GE::OpenGLShader>(m_ColorShader)->Bind();
	//std::dynamic_pointer_cast<GE::OpenGLShader>(m_ColorShader)->SetUniformFloat4("u_Color", m_SquareColor);


}

void Game2D_Layer::OnImGuiRender()
{
	GE_PROFILE_FUNCTION();



	static bool show = true;

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	static bool p_Open = true;

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_Open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}


	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			//// Disabling fullscreen would allow the window to be moved to the front of other windows,
			//// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			//ImGui::MenuItem("Padding", NULL, &opt_padding);
			//ImGui::Separator();

			//if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
			//if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
			//if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
			//if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			//if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			//if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			//ImGui::Separator();

			if (ImGui::MenuItem("Exit", NULL, false))
				GE::Application::Get().CloseApplication();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}


	ImGui::Begin("Setting");
	auto stat = GE::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stat: ");
	ImGui::Text("Draw Call: %d", stat.DrawCall);
	ImGui::Text("Quad Count: %d", stat.QuadCount);
	ImGui::Text("Vertices: %d", stat.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stat.GetTotalIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();

	ImGui::Begin("Tes");
	ImGui::Text("just a friking testing");

	
	
	ImGui::End();

	ImGui::Begin("ViewPort");
	uint32_t FBtextureID = m_FrameBuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)FBtextureID, ImVec2(1280, 720), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	
	ImGui::End();


}

void Game2D_Layer::OnEvent(GE::Event& e)
{
	m_CameraController.OnEvent(e);
}
