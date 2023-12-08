#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"

namespace GE {
	class Renderer2D
	{
	public: 
		static void Init();
		static void ShutDown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		//Rotation is on radians
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size,float rotation, const glm::vec4& color = glm::vec4(1.0f));

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f }, float TilingMultiplier = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f }, float TilingMultiplier = 1.0f);

		struct Statistics {
			uint32_t DrawCall = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static Statistics GetStats();

		static void ResetStats();
		
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;

		};

		static SceneData* m_SceneData;


		static void FlushAndReset();
	};
}


//namespace GE {
//
//	class Renderer2D
//	{
//	public:
//		static void Init();
//		static void Shutdown();
//
//		static void BeginScene(const OrthographicCamera& camera);
//		static void EndScene();
//		static void Flush();
//
//		// Primitives
//		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
//		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
//		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
//		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
//
//		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
//		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
//		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
//		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
//
//		// Stats
//		struct Statistics
//		{
//			uint32_t DrawCalls = 0;
//			uint32_t QuadCount = 0;
//
//			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
//			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
//		};
//		static void ResetStats();
//		static Statistics GetStats();
//	private:
//		static void FlushAndReset();
//	};
//
//}
