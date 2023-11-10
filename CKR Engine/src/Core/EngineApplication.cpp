#include "gepch.h"
#include "EngineApplication.h"


#include "Event/Event.h"
#include "Log.h"
#include <glad/glad.h>
#include "Input.h"


namespace GE {


#define Bind_Event_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	
	//Create window, set window event callback, everytime input trigger, callbackevent will trigger
	GE::Application::Application()
	{
		//GE_CORE_ASSERT(s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(Bind_Event_FN(OnEvent));
		
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//Generate opengl context and store it inside unsign int
		//bind the opengl context to current window
		
		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.5f,0.0f,1.0f, 1.0f, 1.0f, 1.0f,
		};

		
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		
		//Because we setup iterator in the buffer layout, so we can do for loop
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);


		//for loop for the buffer element in the buffer layout.
		//this should be inside vertex array
		/*uint32_t index = 0;
		for (const auto& element : m_VertexBuffer->GetLayout()) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(),
			ShaderDataTypeToOpenGlBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE,
				m_VertexBuffer->GetLayout().GetStrides(), (const void*)element.Offset);
			index++;
		}*/



		//This will input the data into the shader program
		//which will be the position 0 vec3 a_Position under there
		
		
		uint32_t indices[3] = {
			0,1,2
		};
		//the count is 3 because we have 3 element in array
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
		m_VertexArray->AddIndexBuffer(m_IndexBuffer);
		

		/////////////////////////////////////For square 
		m_SquareVA.reset(VertexArray::Create());
		float squarevertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			-0.75f, 0.75f,0.0f,
			0.75f, 0.75f,0.0f
		};


		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squarevertices, sizeof(squarevertices)));

		BufferLayout sqlayout = {
			{ShaderDataType::Float3, "a_Position"},
		};

		squareVB->SetLayout(sqlayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[6] = {
			0,2,1,3,2,1
		};
		//the count is 3 because we have 3 element in array
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		m_SquareVA->AddIndexBuffer(squareIB);


		//vertex handle position, fragment handle color
		std::string vertexSrc = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;
		
		out vec3 v_Position;
		out vec4 v_Color;
		void main(){
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = vec4(a_Position, 1.0);
		}
		)";
		std::string fragmentSrc = R"(
		#version 330 core
		
		layout(location = 0) out vec4 color;
		in vec3 v_Position;
		in vec4 v_Color;

		void main(){
			color = v_Color;
		}
		)";
		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		//For blue square
		std::string vertexSrc2 = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		
		out vec3 v_Position;
		
		void main(){
			v_Position = a_Position;
			gl_Position = vec4(a_Position , 1.0);
		}
		)";
		std::string fragmentSrc2 = R"(
		#version 330 core
		
		layout(location = 0) out vec4 color;
		in vec3 v_Position;
		
		void main(){
			color = vec4(0.2, 0.3, 0.8,1.0);
		}
		)";

		m_Shader2.reset(new Shader(vertexSrc2, fragmentSrc2));

	}

	GE::Application::~Application()
	{
	}

	//While loop for window, constantly update layer
	void Application::Run()
	{
		if (!m_Window) {
			GELog_Fatal("No Window Reference created from application");
		}
		while (m_Running) {

			glClearColor(0.2f, 0.2f, 0.2f, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader2->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
			
			
			//Layer Update in layerstack
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			//to render every layer
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnRender();
			}
			m_ImGuiLayer->End();


			//Window Update
			m_Window->OnUpdate();
			

		}
		
	}

	//Event callback that bind to oneventcallback
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e); 
	
		dispatcher.Dispatch<WindowCloseEvent>(Bind_Event_FN(OnWindowClosed));
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}

		
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}



	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnButtonPressed(MouseButtonEvent& e)
	{
		return true;
	}

}