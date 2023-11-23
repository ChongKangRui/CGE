// CKR Engine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Engine.h>
#include <Core/EngineApplication.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"



#define Bind_Event_FN(x) std::bind(&GE::Application::x, this, std::placeholders::_1)

class ExampleLayer : public GE::Layer {

	
public:
	ExampleLayer() : Layer("Testing"), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f), m_CameraPosition({0.0f}), m_CameraRotation(0.0f), m_TrianglePos(0.0f), m_SquadPos(0.0f){
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


		GE::Ref<VertexBuffer> squareVB;
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
		GE::Ref<IndexBuffer> squareIB;
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
		uniform mat4 u_ModelTransform;

		void main(){
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = u_ViewProjection * u_ModelTransform * vec4(a_Position, 1.0);
			
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
		m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));

		//For blue square
		std::string vertexSrc2 = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		
		out vec3 v_Position;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_ModelTransform;
		

		void main(){
			v_Position = a_Position;
			gl_Position = u_ViewProjection* u_ModelTransform * vec4(a_Position , 1.0);
		}
		)";
		std::string fragmentSrc2 = R"(
		#version 330 core
		
		layout(location = 0) out vec4 color;
		in vec3 v_Position;

		uniform vec4 u_Color;
		
		void main(){
			color = u_Color;
		}
		)";

		m_Shader2.reset(Shader::Create(vertexSrc2, fragmentSrc2));

	};

	void OnUpdate(GE::TimeStep deltatime) override {

		//Log_Trace("Delta time: {0}s {1}ms", deltatime.GetSeconds(), deltatime.GetMilliseconds());
		InputEvent(deltatime);

		GE::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0 });
		GE::RenderCommand::Clear();


		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);


		GE::Renderer::BeginScene(m_Camera);

		glm::mat4 scales = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	
		std::dynamic_pointer_cast<GE::OpenGLShader>(m_Shader2)->Bind();
		std::dynamic_pointer_cast<GE::OpenGLShader>(m_Shader2)->SetUniformFloat4("u_Color", m_SquareColor);


		for (int x = 0; x < 5; x++) {
			for (int y = 0; y < 5; y++) {
				glm::vec3 pos(x * 0.2f, y * 0.2f, 0.0f);

				//if (x % 2 == 0)
				//	std::dynamic_pointer_cast<GE::OpenGLShader>(m_Shader2)->SetUniformFloat4("u_Color", {1.0f,0.5f,1.0f,1.0f});
				//else
				//	std::dynamic_pointer_cast<GE::OpenGLShader>(m_Shader2)->SetUniformFloat4("u_Color", { 0.5f,1.0f,1.0f,1.0f });


				GE::Renderer::Submit(m_Shader2, m_SquareVA, glm::translate(glm::mat4(1.0f), m_TrianglePos + pos) * scales);
			}
		}
		GE::Renderer::Submit(m_Shader, m_VertexArray, glm::translate(glm::mat4(1.0f), m_SquadPos));
		GE::Renderer::EndScene();

	};

	virtual void OnRender() override {
		ImGui::Begin("Setting");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();

	}

	void OnEvent(GE::Event& event) override {

		
	}

	void InputEvent(GE::TimeStep ts) {


		float time = ts;
		float speed = time * m_CameraMoveSpeed;
		if (GE::Input::IsKeyPressed(KEY_TAB))
			Log_Trace("Tab key is pressed (poll)!");
		if (GE::Input::IsKeyPressed(KEY_LEFT))
			m_CameraPosition.x -= speed;
		else if (GE::Input::IsKeyPressed(KEY_RIGHT))
			m_CameraPosition.x += speed;

		if (GE::Input::IsKeyPressed(KEY_UP))
			m_CameraPosition.y += speed;
		else if (GE::Input::IsKeyPressed(KEY_DOWN))
			m_CameraPosition.y -= speed;

		if (GE::Input::IsKeyPressed(KEY_Q))
			m_CameraRotation += speed;
		if (GE::Input::IsKeyPressed(KEY_E))
			m_CameraRotation -= speed;


		if (GE::Input::IsKeyPressed(KEY_W))
			m_TrianglePos.y += speed;
		else if (GE::Input::IsKeyPressed(KEY_S))
			m_TrianglePos.y -= speed;
		if (GE::Input::IsKeyPressed(KEY_A))
			m_TrianglePos.x += speed;
		if (GE::Input::IsKeyPressed(KEY_D))
			m_TrianglePos.x -= speed;


		if (GE::Input::IsKeyPressed(KEY_I))
			m_SquadPos.y += speed;
		else if (GE::Input::IsKeyPressed(KEY_K))
			m_SquadPos.y -= speed;
		if (GE::Input::IsKeyPressed(KEY_J))
			m_SquadPos.x += speed;
		if (GE::Input::IsKeyPressed(KEY_L))
			m_SquadPos.x -= speed;

	}
private:
	GE::Ref<GE::Shader> m_Shader;
	GE::Ref<GE::Shader> m_Shader2;
	GE::Ref<GE::VertexBuffer> m_VertexBuffer;
	GE::Ref<GE::IndexBuffer> m_IndexBuffer;
	GE::Ref<GE::VertexArray> m_VertexArray;

	GE::Ref<GE::VertexArray> m_SquareVA;

	GE::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	glm::vec3 m_TrianglePos;
	glm::vec3 m_SquadPos;

	
	float m_CameraRotation;

	float m_CameraRotationSpeed = 1.0f;
	float m_CameraMoveSpeed = 10.0f;

	glm::vec4 m_SquareColor = { 1,0.3,0.4,1.0f };
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



