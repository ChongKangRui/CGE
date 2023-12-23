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


		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCameraEntity = m_ActiveScene->CreateEntity("second Camera");
		auto& cc = m_SecondCameraEntity.AddComponent<CameraComponent>();
		cc.Primary = false;


		class CameraController : public ScriptableEntity {
		public:
		 void OnCreate() {
				
			}

		 void OnDestroy(){
			}

		 void OnUpdate(Timestep ts) {
				/*auto& pos = GetComponent<TransformComponent>().Position;
				auto& prim = GetComponent<CameraComponent>().Primary;
			
				
				if (prim) {
					if (GE::Input::IsKeyPressed(KEY_A))
						pos -= 5.0 * ts;
					else if (GE::Input::IsKeyPressed(KEY_D))
						pos += 5.0 * ts;


					if (Input::IsKeyPressed(KEY_W))
						pos += 5.0 * ts;
					else if (Input::IsKeyPressed(KEY_S))
						pos -= 5.0 * ts;

				}
			*/
			
			
			}
		};

		m_SecondCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();


		m_SHP.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		GE_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		GE_PROFILE_FUNCTION();

		//m_ActiveScene->OnViewportResize

		//////Move from imgui render update to on update so can fix the flickering camera issue when resize
		//Flickering is because recreate the color attachment, then right next draw using it before new texture is drawn to.
		//Maybe first draw the viewport, even if the size is wrong, then resize if needed.Would flicker less.
		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y)) 
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		}

		if (m_ViewportFocus)
			m_CameraController.OnUpdate(ts);

		Renderer2D::ResetStats();

		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0 });
		RenderCommand::Clear();


		//Update scene
		m_ActiveScene->OnUpdate(ts);




		//Renderer2D::EndScene();
		m_FrameBuffer->Unbind();


		
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


				if (ImGui::MenuItem("Exit", NULL, false))
					Application::Get().CloseApplication();
				ImGui::EndMenu();
			}




			ImGui::EndMenuBar();
		}


		m_SHP.OnImGuiRender();

		ImGui::Begin("Setting");
		auto stat = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stat: ");
		ImGui::Text("Draw Call: %d", stat.DrawCall);
		ImGui::Text("Quad Count: %d", stat.QuadCount);
		ImGui::Text("Vertices: %d", stat.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stat.GetTotalIndexCount());

		/*if (m_SquareTest) {
			auto& squareColor = m_SquareTest.GetComponent<SpriteComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
		}*/

		/*ImGui::DragFloat3("Camera Transform", 
			glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));

		if (ImGui::Checkbox("Camera A", &primaryCamera)) {
			auto& cc = m_SecondCameraEntity.GetComponent<CameraComponent>();
			cc.Primary = !primaryCamera;

			auto& c1 = m_CameraEntity.GetComponent<CameraComponent>();
			c1.Primary = primaryCamera;

		}


		{
			auto& cc = m_SecondCameraEntity.GetComponent<CameraComponent>();
			float orthosize = cc.Camera.GetOrthographicSize();
			if(ImGui::DragFloat("SceoncCam ortho size", &orthosize)){
				cc.Camera.SetOrthographicSize(orthosize);
			}
		}*/

		

		ImGui::End();

	

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

		//////////////////////////viewport
		ImGui::Begin("ViewPort");

		m_ViewportFocus = ImGui::IsWindowFocused();
		m_ViewportHover = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocus || !m_ViewportHover);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };


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
