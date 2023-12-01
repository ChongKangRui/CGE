#include "gepch.h"
#include "Renderer2D.h"
#include "RendererCommand.h"

#include "VertexArray.h"
#include "Shader.h"

//#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"


namespace GE {

	struct Renderer2DStorage {
		Ref<VertexArray> QuadVA;
		Ref<Shader> ColorShader;
		Ref<Shader> TextureShader;
	};

	//can make this as pointer if we have a bunch of data wanna manage
	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->QuadVA = VertexArray::Create();
		float squarevertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
			-0.5f, 0.5f,0.0f, 0.0f, 1.0f,
			0.5f, 0.5f,0.0f, 1.0f, 1.0f,
		};


		Ref<VertexBuffer> squareVB = (VertexBuffer::Create(squarevertices, sizeof(squarevertices)));

		//Expand our layout so stride can get the correct offset
		BufferLayout sqlayout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};

		squareVB->SetLayout(sqlayout);
		s_Data->QuadVA->AddVertexBuffer(squareVB);

		uint32_t indices2[6] = {
			0,2,1,3,2,1
		};
		//the count is 3 because we have 3 element in array
		Ref<IndexBuffer> squareIB = IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t));
		s_Data->QuadVA->AddIndexBuffer(squareIB);

		s_Data->ColorShader= Shader::Create("assets/Shader/ColorShader.glsl");
		s_Data->TextureShader = Shader::Create("assets/Shader/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}
	void Renderer2D::ShutDown()
	{
		delete s_Data;
	}
	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_Data->ColorShader->Bind();
		s_Data->ColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());


	}
	void Renderer2D::EndScene()
	{
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0 }, size, color);

	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->ColorShader->Bind();
		s_Data->ColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)* glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		

		s_Data->ColorShader->SetMat4("u_ModelTransform", transform);


		
		s_Data->QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVA);

	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0 }, size, texture);

	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_ModelTransform", transform);
		texture->Bind();


		s_Data->QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVA);
	}
}