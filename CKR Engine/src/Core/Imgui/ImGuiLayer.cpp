#include "gepch.h"
#include "ImGuiLayer.h"
//#include "ImguiBuild.cpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Core/Base/EngineApplication.h"


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

		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags && ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
	
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

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		/*ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();*/
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
	//void ImGuiLayer::OnEvent(Event& event)
	//{
	//	EventDispatcher dispatcher(event);

	//	//All the event that bind into dispatcher in order to trigger specific event for imgui
	//	//More binding explanation can look at EngineApplication.h which involve bind to glfw input setup
	//	dispatcher.Dispatch<MouseButtonPressedEvent>(GE_BEVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
	//	dispatcher.Dispatch<MouseButtonReleasedEvent>(GE_BEVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
	//	dispatcher.Dispatch<MouseMovedEvent>(GE_BEVENT_FN(ImGuiLayer::OnMouseMovedEvent));
	//	dispatcher.Dispatch<MouseScrolledEvent>(GE_BEVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
	//	dispatcher.Dispatch<KeyPressedEvent>(GE_BEVENT_FN(ImGuiLayer::OnKeyPressedEvent));
	//	dispatcher.Dispatch<KeyReleasedEvent>(GE_BEVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
	//	dispatcher.Dispatch<WindowResizeEvent>(GE_BEVENT_FN(ImGuiLayer::OnWindowResizedEvent));
	//	dispatcher.Dispatch<KeyTypeEvent>(GE_BEVENT_FN(ImGuiLayer::OnKeyTypeEvent));

	//}
	//bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.MouseDown[e.GetMouseButton()] = true;

	//	return false;

	//}
	//bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.MouseDown[e.GetMouseButton()] = false;

	//	return false;
	//}
	//bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.MousePos = ImVec2(e.GetX(), e.GetY());

	//	return false;
	//}
	//bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.MouseWheelH += e.GetXOffset();
	//	io.MouseWheel += e.GetYOffset();

	//	return false;

	//}
	//bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.KeysDown[e.GetKeyCode()] = true;

	//	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	//	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	//	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	//	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	//	return false;
	//}
	//bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.KeysDown[e.GetKeyCode()] = false;
	//	return false;
	//}
	//bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
	//	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	//	glViewport(0,0,e.GetWidth(), e.GetHeight());

	//	return false;
	//}
	//bool ImGuiLayer::OnKeyTypeEvent(KeyTypeEvent& e)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	int keycode = e.GetKeyCode();

	//	

	//	if (keycode > 0 && keycode < 0x10000)
	//		io.AddInputCharacter((unsigned short)keycode);

	//	return false;
	//}
}
