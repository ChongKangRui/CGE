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

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);


	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;

		};

		static SceneData* m_SceneData;
	};
}

