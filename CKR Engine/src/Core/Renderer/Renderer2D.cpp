#include "gepch.h"
#include "Renderer2D.h"
#include "RendererCommand.h"

#include "VertexArray.h"
#include "Shader.h"

//#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"



namespace GE {


	struct QuadVertex {

		glm::vec3 Position;
		glm::vec4 color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		// Todo: color, texid

	};


	struct Renderer2DData {
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;//Todo RenderCaps

		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white Texture
	};

	//can make this as pointer if we have a bunch of data wanna manage
	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		GE_PROFILE_FUNCTION();

		s_Data.QuadVA = VertexArray::Create();
		s_Data.QuadVB = (VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex)));

		//Expand our layout so stride can get the correct offset
		BufferLayout sqlayout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_Tiling"}
		};

		s_Data.QuadVB->SetLayout(sqlayout);
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;

		//int test = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i+= 6) {
			quadIndices[i] = offset;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			
			//GELog_Info("The maxindices loop {0}", test);
			offset += 4;
		}


		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices* sizeof(uint32_t));
		s_Data.QuadVA->AddIndexBuffer(quadIB);

		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[32];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader = Shader::Create("assets/Shader/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);


		//Init texture slot
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		
	}
	void Renderer2D::ShutDown()
	{
		GE_PROFILE_FUNCTION();
	}
	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		GE_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}
	void Renderer2D::EndScene()
	{
		GE_PROFILE_FUNCTION();

		uint32_t datasize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, datasize);


		Flush();
	}
	void Renderer2D::Flush()
	{
		GE_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVA,s_Data.QuadIndexCount);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0 }, size, rotation, color);

	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		GE_PROFILE_FUNCTION();

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y,  position.z};
		s_Data.QuadVertexBufferPtr->color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };;
		s_Data.QuadVertexBufferPtr->color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;


#if old_drawQuad
		/*s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat1("u_TilingFactor", 1.0f);
		s_Data.WhiteTexture->Bind();

		auto PosMatrix = glm::translate(glm::mat4(1.0f), position);
		auto RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
		auto ScaleMatrix = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		glm::mat4 transform = PosMatrix * RotationMatrix * ScaleMatrix;
		s_Data.TextureShader->SetMat4("u_ModelTransform", transform);


		
		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA);
		s_Data.WhiteTexture->Unbind();*/
#endif
	}
	

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color, float TilingMultiplier)
	{
	
		DrawQuad({ position.x, position.y, 0 }, size, rotation, texture, color, TilingMultiplier);

	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color, float TilingMultiplier)
	{
		GE_PROFILE_FUNCTION();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;

			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;

		}

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingMultiplier;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y,  position.z };
		s_Data.QuadVertexBufferPtr->color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingMultiplier;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingMultiplier;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };;
		s_Data.QuadVertexBufferPtr->color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingMultiplier;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

#if old_drawQuad


		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat1("u_TilingFactor", TilingMultiplier);
		texture->Bind();

		auto PosMatrix = glm::translate(glm::mat4(1.0f), position);
		auto RotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
		auto ScaleMatrix = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		glm::mat4 transform = PosMatrix * RotationMatrix * ScaleMatrix;
		s_Data.TextureShader->SetMat4("u_ModelTransform", transform);
		
		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);

		texture->Unbind();
#endif
	}
}