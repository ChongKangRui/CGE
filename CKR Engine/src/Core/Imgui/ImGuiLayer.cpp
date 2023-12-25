#include "gepch.h"
#include "ImGuiLayer.h"
//#include "ImguiBuild.cpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Core/Base/EngineApplication.h"

#include "Core/Event/Event.h"


//Temporary for testing
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace GE {
	ImGuiLayer::ImGuiLayer() : Layer("IMGUI Layer")
	{

	}
	ImGuiLayer::~ImGuiLayer()
	{

	}
	void ImGuiLayer::OnAttach()
	{
		GE_PROFILE_FUNCTION();

		//set up dear imgui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


		//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;


		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans_Condensed-Bold.ttf", 18.0f);
		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags && ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
	
		SetDarkThemeColor();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());


		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		
		
	}
	void ImGuiLayer::OnDetach()
	{
		GE_PROFILE_FUNCTION();

		//clean up
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	
	void ImGuiLayer::OnImGuiRender()
	{
		GE_PROFILE_FUNCTION();

		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);

		/*ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();*/
	}
	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents) {
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
		 
	}
	void ImGuiLayer::Begin()
	{
		GE_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void ImGuiLayer::End()
	{
		GE_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_Current_Context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_Current_Context);

		}
		
	
	
	}

	void ImGuiLayer::SetDarkThemeColor()
	{
		auto& color = ImGui::GetStyle().Colors;

		color[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);

		//Header
		color[ImGuiCol_Header] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_HeaderHovered] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_HeaderActive] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);


		color[ImGuiCol_Button] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);

		color[ImGuiCol_FrameBg] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_FrameBgHovered] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_FrameBgActive] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);

		color[ImGuiCol_Tab] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_TabHovered] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_TabActive] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_TabUnfocused] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);

		color[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_TitleBgActive] = ImVec4(0.5f, 0.105f, 0.11f, 1.0f);
		color[ImGuiCol_TitleBgCollapsed] = ImVec4(1, 0.105f, 0.11f, 1.0f);
	}

}
