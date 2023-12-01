#pragma once
#include "Engine.h"

class Game2D_Layer : public GE::Layer
{
public:
	Game2D_Layer();
	virtual ~Game2D_Layer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(GE::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(GE::Event& e) override;

private:
	GE::OrthographicCameraController m_CameraController;

	//temp
	GE::ShaderLibrary m_ShaderLibrary;
	GE::Ref<GE::Shader> m_ColorShader;
	//GE::Ref<GE::Shader> m_TextureShader;

	GE::Ref<GE::Texture2D> m_Texture2DExample;
	

	GE::Ref<GE::VertexArray> m_SquareVA;


	glm::vec3 m_SquadPos = { 0.0f, 0.0f, 0.0f };
	float m_Rotation = 0;

	glm::vec4 m_SquareColor = { 1,0.3,0.4,1.0f };
};

