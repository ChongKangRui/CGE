#pragma once
#include "Engine.h"



namespace GE {

	

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;


	private:
		OrthographicCameraController m_CameraController;

		//temp
		ShaderLibrary m_ShaderLibrary;
		Ref<Shader> m_ColorShader;
		//Ref<Shader> m_TextureShader;

		Ref<Texture2D> m_TextureTree;
		Ref<Texture2D> m_SpriteSheet;

		Ref<VertexArray> m_SquareVA;

		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareTest;

		glm::vec3 m_SquadPos = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0;

		glm::vec4 m_SquareColor = { 1,0.3,0.4,1.0f };

		glm::vec2 m_ViewportSize;

		bool m_ViewportFocus = false, m_ViewportHover = false;

		
	};
}
