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

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size,float rotation, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f }, float TilingMultiplier = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f }, float TilingMultiplier = 1.0f);


	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;

		};

		static SceneData* m_SceneData;
	};
}

