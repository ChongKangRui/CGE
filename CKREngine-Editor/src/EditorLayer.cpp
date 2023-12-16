#include "EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include "Core/Scene/Entity.h"

namespace GE {
	EditorLayer::EditorLayer() : Layer("Application 2D Renderer"), m_CameraController(1280 / 720, true)
	{

	}

	void EditorLayer::OnAttach()
	{
		GE_PROFILE_FUNCTION();

		m_TextureTree = Texture2D::Create("assets/Textures/TreeMat.png");
		m_SpriteSheet = Texture2D::Create("assets/Textures/RPGpack_sheet.png");
		//m_Subtexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7,6 }, { 128/2,128/2 });


		FrameBufferSpecification spec;
		spec.width = 1280;
		spec.height = 720;

		m_FrameBuffer = FrameBuffer::Create(spec);

		m_ActiveScene = CreateRef<Scene>();

		m_SquareTest = m_ActiveScene->CreateEntity("Square");

		m_SquareTest.AddComponent<SpriteComponent>(glm::vec4(1.0f,0.0f,0.0f,1.0f));

	
	}

	void EditorLayer::OnDetach()
	{
		GE_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
		GE_PROFILE_FUNCTION();



		if (m_ViewportFocus)
			m_CameraController.OnUpdate(ts);

		Renderer2D::ResetStats();

		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0 });
		RenderCommand::Clear();



		//Temp for testing only
		if (Input::IsKeyPressed(KEY_I))
			m_SquadPos.y += 1 * ts;
		else if (Input::IsKeyPressed(KEY_K))
			m_SquadPos.y -= 1 * ts;
		if (Input::IsKeyPressed(KEY_J))
			m_SquadPos.x -= 1 * ts;
		else if (Input::IsKeyPressed(KEY_L))
			m_SquadPos.x += 1 * ts;

		if (Input::IsKeyPressed(KEY_U))
			m_Rotation += 1 * ts;
		else if (Input::IsKeyPressed(KEY_O))
			m_Rotation -= 1 * ts;




		Renderer2D::BeginScene(m_CameraController.GetCamera());

		//Update scene
		m_ActiveScene->OnUpdate(ts);

		Renderer2D::EndScene();
		m_FrameBuffer->Unbind();







		//Todo Shader::SetMAt4, Shader::SetFloat4 etc;
		//Renderer::Submit(m_ColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
		//std::dynamic_pointer_cast<OpenGLShader>(m_ColorShader)->Bind();
		//std::dynamic_pointer_cast<OpenGLShader>(m_ColorShader)->SetUniformFloat4("u_Color", m_SquareColor);


	}

	void EditorLayer::OnImGuiRender()
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
					Application::Get().CloseApplication();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}




		ImGui::Begin("Setting");
		auto stat = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stat: ");
		ImGui::Text("Draw Call: %d", stat.DrawCall);
		ImGui::Text("Quad Count: %d", stat.QuadCount);
		ImGui::Text("Vertices: %d", stat.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stat.GetTotalIndexCount());

		if (m_SquareTest) {
			auto& squareColor = m_SquareTest.GetComponent<SpriteComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
		}
		ImGui::End();

		ImGui::Begin("Tes");
		ImGui::Text("just a friking testing");



		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

		//////////////////////////viewport
		ImGui::Begin("ViewPort");

		m_ViewportFocus = ImGui::IsWindowFocused();
		m_ViewportHover = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocus || !m_ViewportHover);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != *(glm::vec2*)&viewportPanelSize && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {

			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);


			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);


		}

		uint32_t FBtextureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)FBtextureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();


		ImGui::End();


	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}
