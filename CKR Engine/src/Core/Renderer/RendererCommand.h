#pragma once
#include "RendererAPI.h"

namespace GE {
	class RenderCommand
	{

	public:
		inline static void Init() { s_RendererAPI->Init(); };

		inline static void SetViewPort(uint32_t width, uint32_t height) {
			s_RendererAPI->SetViewPort(0,0,width, height);
		}

		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); };
		inline static void Clear() { s_RendererAPI->Clear(); };

		

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t IndexCount) {
			s_RendererAPI->DrawIndexed(vertexArray, IndexCount);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}

