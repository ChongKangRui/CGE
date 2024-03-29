#include "gepch.h"
#include "Renderer2D.h"
#include "RendererCommand.h"
#include "Core/Renderer/UniformBuffer.h"

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
		
		//Editor only
		int EntityID = 0;


	};


	struct Renderer2DData {
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
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

		glm::vec4 QuadVertexPosition[4];

	
		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	//can make this as pointer if we have a bunch of data wanna manage
	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		GE_PROFILE_FUNCTION();

		s_Data.QuadVA = VertexArray::Create();
		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		//Expand our layout so stride can get the correct offset
		BufferLayout sqlayout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_Tiling"},
			{ShaderDataType::Int, "a_EntityID"}
		};

		s_Data.QuadVB->SetLayout(sqlayout);
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;

		//int test = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i+= 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			
			//GELog_Info("The maxindices loop {0}", test);
			offset += 4;
		}


		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVA->AddIndexBuffer(quadIB);

		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}
		s_Data.TextureShader = Shader::Create("assets/Shader/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);


		//Init texture slot
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		s_Data.QuadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);


	}
	void Renderer2D::ShutDown()
	{
		GE_PROFILE_FUNCTION();
	}
	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		GE_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("Camera.u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();


	}
	void Renderer2D::BeginScene(EditorCamera& camera)
	{
		GE_PROFILE_FUNCTION();
		
		glm::mat4 viewProj = camera.GetViewProjectionMatrix();

		//s_Data.TextureShader->Bind();
		//s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);
		s_Data.CameraBuffer.ViewProjection = viewProj;
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();

	}
	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		GE_PROFILE_FUNCTION();

		//s_Data.TextureShader->Bind();
		//s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);
		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}
	void Renderer2D::EndScene()
	{
		GE_PROFILE_FUNCTION();

		uint32_t datasize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, datasize);


		//GELog_Info("Base {0}, PTr {1}", s_Data.QuadVertexBufferBase->Position.x, )

		Flush();
	}
	void Renderer2D::Flush()
	{
		GE_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);
		}
		s_Data.TextureShader->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA,s_Data.QuadIndexCount);
		s_Data.Stats.DrawCall++;
	
	}


	void Renderer2D::FlushAndReset() {
		EndScene();
		//Reset everything
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}


	void Renderer2D::DrawSprite(int EntityId, const glm::mat4& transform, SpriteComponent& src)
	{
		if (src.Texture)
			DrawSprite(EntityId, transform, src.Texture, src.Color, src.TilingFactor);
		else
			DrawSprite(EntityId, transform, src.Color);
	}

	void Renderer2D::DrawSprite(int EntityId, const glm::mat4& transform, const glm::vec4& color)
	{
		GE_PROFILE_FUNCTION();

		//If exceed max indices, start a new drawcall
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();

		}
		const float TilingFactor = 1.0f;

		//WhiteTexture
		const float texIndex = 0.0f;

		constexpr glm::vec2 TexCoord[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },{ 0.0f, 1.0f } };

		constexpr size_t quadVertexCount = 4;

		for (size_t i = 0; i < quadVertexCount; i++) {

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = TexCoord[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = EntityId;
			s_Data.QuadVertexBufferPtr++;

		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawSprite(int EntityId, const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, int tilingFactor)
	{
		GE_PROFILE_FUNCTION();

		//If exceed max indices, start a new drawcall
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();

		}

		float texIndex = 0;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				texIndex = (float)i;

				break;
			}
		}


		if (texIndex == 0.0f) {
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;

		}

		constexpr glm::vec2 TexCoord[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },{ 0.0f, 1.0f } };

		constexpr size_t quadVertexCount = 4;

		for (size_t i = 0; i < quadVertexCount; i++) {

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = TexCoord[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = EntityId;
			s_Data.QuadVertexBufferPtr++;

		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	//void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	//{
	//	DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);

	//}

	//
	//void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	//{
	//	GE_PROFILE_FUNCTION();


	//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
	//		glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
	//		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	//	
	//	DrawQuad(transform, color);

	//}

	//void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	//{
	//	GE_PROFILE_FUNCTION();

	//	//If exceed max indices, start a new drawcall
	//	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
	//		FlushAndReset();

	//	}

	//	const float TilingFactor = 1.0f;

	//	//WhiteTexture
	//	const float texIndex = 0.0f;

	//	//GELog_Info("{0}", position);

	//
	//	constexpr glm::vec2 TexCoord[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },{ 0.0f, 1.0f } };

	//	constexpr size_t quadVertexCount = 4;

	//	for (size_t i = 0; i < quadVertexCount; i++) {

	//		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
	//		s_Data.QuadVertexBufferPtr->color = color;
	//		s_Data.QuadVertexBufferPtr->TexCoord = TexCoord[i];
	//		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
	//		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
	//		s_Data.QuadVertexBufferPtr->EntityID = 0;
	//		s_Data.QuadVertexBufferPtr++;

	//	}

	//	s_Data.QuadIndexCount += 6;

	//	s_Data.Stats.QuadCount++;


	//}
	//

	//void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color, float TilingMultiplier)
	//{
	//
	//	DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, color, TilingMultiplier);

	//}
	//void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color, float TilingMultiplier)
	//{
	//	GE_PROFILE_FUNCTION();

	//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
	//		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) *
	//		glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f });

	//	DrawQuad(transform, texture,color,TilingMultiplier);

	//}

	//void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, float TilingMultiplier)
	//{
	//	GE_PROFILE_FUNCTION();

	//	constexpr glm::vec2 TexCoord[] = {
	//		{ 0.0f, 0.0f },
	//		{ 1.0f, 0.0f },
	//		{ 1.0f, 1.0f },
	//		{ 0.0f, 1.0f } };

	//	constexpr size_t quadVertexCount = 4;

	//	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
	//		FlushAndReset();
	//	}

	//	float textureIndex = 0.0f;

	//	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
	//		if (*s_Data.TextureSlots[i].get() == *texture.get()) {
	//			textureIndex = (float)i;
	//			break;
	//		}
	//	}

	//	if (textureIndex == 0.0f) {
	//		textureIndex = (float)s_Data.TextureSlotIndex;

	//		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
	//		s_Data.TextureSlotIndex++;

	//	}



	//	for (size_t i = 0; i < quadVertexCount; i++) {

	//		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
	//		s_Data.QuadVertexBufferPtr->color = color;
	//		s_Data.QuadVertexBufferPtr->TexCoord = TexCoord[i];
	//		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	//		s_Data.QuadVertexBufferPtr->TilingFactor = TilingMultiplier;
	//		s_Data.QuadVertexBufferPtr->EntityID = 0;
	//		s_Data.QuadVertexBufferPtr++;

	//	}
	//	s_Data.QuadIndexCount += 6;

	//	s_Data.Stats.QuadCount++;
	//}

	//void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& texture, const glm::vec4& color , float TilingMultiplier) {
	//	DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, color, TilingMultiplier);
	//}
	//void Renderer2D::
	// (const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& color, float TilingMultiplier ) {
	//	GE_PROFILE_FUNCTION();

	//	const glm::vec2* TexCoord = subTexture->GetTexCoords();
	//	const Ref<Texture2D> texture = subTexture->GetTexture();

	//	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
	//		FlushAndReset();
	//	}

	//	float textureIndex = 0.0f;

	//	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
	//		if (*s_Data.TextureSlots[i].get() == *texture.get()) {
	//			textureIndex = (float)i;
	//			break;
	//		}
	//	}

	//	if (textureIndex == 0.0f) {
	//		textureIndex = (float)s_Data.TextureSlotIndex;

	//		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
	//		s_Data.TextureSlotIndex++;

	//	}

	//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
	//		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) *
	//		glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f });

	//	

	//	constexpr size_t quadVertexCount = 4;

	//	for (size_t i = 0; i < quadVertexCount; i++) {

	//		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
	//		s_Data.QuadVertexBufferPtr->color = color;
	//		s_Data.QuadVertexBufferPtr->TexCoord = TexCoord[i];
	//		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	//		s_Data.QuadVertexBufferPtr->TilingFactor = TilingMultiplier;
	//		s_Data.QuadVertexBufferPtr->EntityID = 0;
	//		s_Data.QuadVertexBufferPtr++;

	//	}
	//	s_Data.QuadIndexCount += 6;

	//	s_Data.Stats.QuadCount++;

	//}



	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}
}

