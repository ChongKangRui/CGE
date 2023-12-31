#include "EditorLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ImGuizmo.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "Core/Scene/Entity.h"

#include "Core/Scene/SceneSerializer.h"
#include "Core/Utils/PlatformUtils.h"

#include "Core/Math/Math.h"

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


		FramebufferSpecification fbspec;
		fbspec.Attachments = { FramebufferTextureFormat::RGBA8,FramebufferTextureFormat::RED_INTERGER ,FramebufferTextureFormat::Depth };
		fbspec.Width = 1280;
		fbspec.Height = 720;

		m_FrameBuffer = FrameBuffer::Create(fbspec);

		m_ActiveScene = CreateRef<Scene>();


#if 0
		m_SquareTest = m_ActiveScene->CreateEntity("Square");

		m_SquareTest.AddComponent<SpriteComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));


		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCameraEntity = m_ActiveScene->CreateEntity("second Camera");
		auto& cc = m_SecondCameraEntity.AddComponent<CameraComponent>();
		cc.Primary = false;


		class CameraController : public ScriptableEntity {
		public:
			void OnCreate() {

			}

			void OnDestroy() {
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

#endif


		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_EditorCamera = EditorCamera(30.0f, 1.788, 0.1f, 1000.0f);

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
		if (FramebufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

		}

		if (m_ViewportFocus) {
			m_CameraController.OnUpdate(ts);

		}
		m_EditorCamera.OnUpdate(ts);

		Renderer2D::ResetStats();

		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0 });
		RenderCommand::Clear();

		//Clear entity id into -1
		m_FrameBuffer->ClearAttachment(1, -1);


		//Update scene
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;

		auto viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mousex = (int)mx;
		int mousey = (int)my - 40;

		if (mousex >= 0 && mousey >= 0 && mousex < (int)viewportSize.x && mousey < (int)viewportSize.y) {
			auto pixel = m_FrameBuffer->ReadPixel(1, mousex, mousey);
			if (pixel == -1)
				m_HoveredEntity = {};
			else
				m_HoveredEntity = { (entt::entity)pixel, m_ActiveScene.get() };

			GELog_Info("pixel = {0}", pixel);
		}


		//GELog_Info("mouse pos = {0}, {1}", mousex, mousey);
		//GELog_Info("viewportSize = {0}, {1}", m_ViewportBounds[1].x, m_ViewportBounds[1].y);


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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizex = style.WindowMinSize.x;

		style.WindowMinSize.x = 100.0f;


		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizex;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("New", "Ctrl+N")) {
					NewScene();
				}

				if (ImGui::MenuItem("Open ...", "Ctrl+O")) {
					LoadScene();
				}


				if (ImGui::MenuItem("Save as ...", "Ctrl+Shift+S", false)) {
					SaveScene();
				}

				/*if (ImGui::MenuItem("Serialize", NULL, false)) {
					SceneSerializer serializer(m_ActiveScene);
					serializer.Serialize("assets/scenes/Example.GE");
				}*/

				if (ImGui::MenuItem("Exit", NULL, false))
					Application::Get().CloseApplication();
				ImGui::EndMenu();
			}




			ImGui::EndMenuBar();
		}


		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Setting");
		auto stat = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stat: ");
		ImGui::Text("Draw Call: %d", stat.DrawCall);
		ImGui::Text("Quad Count: %d", stat.QuadCount);
		ImGui::Text("Vertices: %d", stat.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stat.GetTotalIndexCount());



		ImGui::End();



		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

		//////////////////////////viewport///////////////////
		ImGui::Begin("ViewPort");
		auto viewportOffset = ImGui::GetCursorPos(); // include tab bar

		m_ViewportFocus = ImGui::IsWindowFocused();
		m_ViewportHover = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocus && !m_ViewportHover);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };


		uint32_t FBtextureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)FBtextureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();

		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound{ minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };

		/*GELog_Info("Min Bound = {0}, {1}", m_ViewportBounds[0].x, m_ViewportBounds[0].y);
		GELog_Info("Max Bound = {0}, {1}", m_ViewportBounds[1].x, m_ViewportBounds[1].y);*/

		///////////////////Gizmos Rendering/////////////////////
		// In the future should replace to mouse picking entity
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();


		if (selectedEntity && m_GizmodeType != -1) {
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			//Scene Camera
			//Runtime Camera From Entity
			/*auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>();
			auto cameraProjection = camera.Camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

			//Editor Camera
			auto cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();


			//Snaping
			bool snap = Input::IsKeyPressed(KEY_LEFT_CONTROL);
			float snapValue = 0.5f;
			if (m_GizmodeType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			// Entity Transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			auto transform = tc.GetTransform();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmodeType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 position, rotation, scale;
				Math::DecomposeTransform(transform, position, rotation, scale);

				tc.Position = position;
				tc.Rotation = rotation;
				tc.Scale = scale;

			}
		}


		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();


	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(GE_BEVENT_FN(EditorLayer::OnKeyPressed));
	}
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool ctrl = Input::IsKeyPressed(KEY_LEFT_CONTROL) || Input::IsKeyPressed(KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(KEY_LEFT_SHIFT) || Input::IsKeyPressed(KEY_RIGHT_SHIFT);
		
		switch (e.GetKeyCode()) {
		case KEY_S:
		{
			if (ctrl && shift) {
				SaveScene();
			}
			break;
		}
		case KEY_N:
		{
			if (ctrl) {
				NewScene();
			}
			break;
		}
		case KEY_O:
		{
			if (ctrl) {
				LoadScene();
			}
			break;
		}

		}

	///Gizmos Control
	bool RightM = Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
	if (m_SceneHierarchyPanel.GetSelectedEntity() && !RightM) {
		switch (e.GetKeyCode()) {

			case KEY_Q:
			{
				m_GizmodeType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case KEY_W:
			{
				m_GizmodeType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case KEY_E:
			{
				m_GizmodeType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}
	}
		return false;
	}
	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::LoadScene()
	{
		std::string filepath = FileDialogs::OpenFile("Scene (*.GE)\0*.GE\0");
		if (!filepath.empty()) {
			m_ActiveScene = CreateRef<Scene>();
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
	}
	void EditorLayer::SaveScene()
	{
		std::string filepath = FileDialogs::SaveFile("Scene (*.GE)\0*.GE\0");
		if (!filepath.empty()) {
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}


	}
}
