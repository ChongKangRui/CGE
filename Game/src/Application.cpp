// CKR Engine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Engine.h>
#include <Core/EngineApplication.h>


#define Bind_Event_FN(x) std::bind(&GE::Application::x, this, std::placeholders::_1)

class ExampleLayer : public GE::Layer {

	
public:
	ExampleLayer() : Layer("Testing"), m_Camera(-3.0f, 3.0f, -3.0f, 3.0f), m_CameraPosition({0.0f}), m_CameraRotation(0.0f) {
		using namespace GE;
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.5f,0.0f,1.0f, 1.0f, 1.0f, 1.0f,
		};
		m_VertexBuffer.reset(GE::VertexBuffer::Create(vertices, sizeof(vertices)));
		//Because we setup iterator in the buffer layout, so we can do for loop
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};

		m_VertexBuffer->SetLayout(layout);
		uint32_t indices[3] = {
			0,1,2
		};

		//the count is 3 because we have 3 element in array
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		//////Vertex Array
		m_VertexArray.reset(VertexArray::Create());
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
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
		
		uniform mat4 u_ViewProjection;

		void main(){
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			
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

		uniform mat4 u_ViewProjection;
		
		void main(){
			v_Position = a_Position;
			gl_Position = u_ViewProjection* vec4(a_Position , 1.0);
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

	};
	void OnUpdate() override {
		
	}

	virtual void OnRender() override {

		GE::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0 });
		GE::RenderCommand::Clear();


		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);


		GE::Renderer::BeginScene(m_Camera);
		InputEvent();

		GE::Renderer::Submit(m_Shader2, m_SquareVA);
		GE::Renderer::Submit(m_Shader, m_VertexArray);


		GE::Renderer::EndScene();

	}

	void OnEvent(GE::Event& event) override {

		
	}

	void InputEvent() {

		if (GE::Input::IsKeyPressed(KEY_TAB))
			Log_Trace("Tab key is pressed (poll)!");
		if (GE::Input::IsKeyPressed(KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed;
		else if (GE::Input::IsKeyPressed(KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed ;

		if (GE::Input::IsKeyPressed(KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed ;
		else if (GE::Input::IsKeyPressed(KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed ;

		if (GE::Input::IsKeyPressed(KEY_A))
			m_CameraRotation += m_CameraRotationSpeed;
		if (GE::Input::IsKeyPressed(KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed;




	}
private:
	std::shared_ptr<GE::Shader> m_Shader;
	std::shared_ptr<GE::Shader> m_Shader2;
	std::shared_ptr<GE::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<GE::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<GE::VertexArray> m_VertexArray;

	std::shared_ptr<GE::VertexArray> m_SquareVA;

	GE::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation;

	float m_CameraRotationSpeed = 1.0f;
	float m_CameraMoveSpeed = 0.01f;
};


class GameApplication : public GE::Application {
public:
	GameApplication() {
		PushLayer(new ExampleLayer());
		//PushOverlay(new GE::ImGuiLayer());
	}
	~GameApplication(){

	}

};

GE::Application* GE::CreateApplication() {
	return new GameApplication();
}



