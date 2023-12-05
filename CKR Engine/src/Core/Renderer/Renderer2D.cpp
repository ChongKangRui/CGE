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
		
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	//can make this as pointer if we have a bunch of data wanna manage
	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		GE_PROFILE_FUNCTION();

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

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		//
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));


		s_Data->TextureShader = Shader::Create("assets/Shader/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}
	void Renderer2D::ShutDown()
	{
		GE_PROFILE_FUNCTION();


		delete s_Data;
	}
	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		GE_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());


	}
	void Renderer2D::EndScene()
	{
		GE_PROFILE_FUNCTION();

	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0 }, size, rotation, color);

	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		GE_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat1("u_TilingFactor", 1.0f);
		s_Data->WhiteTexture->Bind();

		auto PosMatrix = glm::translate(glm::mat4(1.0f), position);
		auto RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
		auto ScaleMatrix = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		glm::mat4 transform = PosMatrix * RotationMatrix * ScaleMatrix;
		s_Data->TextureShader->SetMat4("u_ModelTransform", transform);


		
		s_Data->QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVA);
		s_Data->WhiteTexture->Unbind();
	}
	

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color, float TilingMultiplier)
	{
	
		DrawQuad({ position.x, position.y, 0 }, size, rotation, texture, color, TilingMultiplier);

	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color, float TilingMultiplier)
	{
		GE_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat1("u_TilingFactor", TilingMultiplier);
		texture->Bind();

		auto PosMatrix = glm::translate(glm::mat4(1.0f), position);
		auto RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
		auto ScaleMatrix = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		glm::mat4 transform = PosMatrix * RotationMatrix * ScaleMatrix;
		s_Data->TextureShader->SetMat4("u_ModelTransform", transform);
		
		s_Data->QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVA);

		texture->Unbind();
	}
}