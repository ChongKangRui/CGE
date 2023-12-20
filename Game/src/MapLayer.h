#pragma once
#include "Engine.h"
#include "ParticleSystem.h"



class Map_Layer : public GE::Layer
{
public:
	Map_Layer();
	virtual ~Map_Layer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(GE::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(GE::Event& e) override;


private:
	GE::OrthographicCameraController m_CameraController;

	//temp
	GE::ShaderLibrary m_ShaderLibrary;
	GE::Ref<GE::Shader> m_ColorShader;
	//GE::Ref<GE::Shader> m_TextureShader;

	GE::Ref<GE::Texture2D> m_Texture2DExample;
	GE::Ref<GE::Texture2D> m_SpriteSheet;

	GE::Ref<GE::SubTexture2D> m_TextureStairs;
	GE::Ref<GE::SubTexture2D> m_TextureTree;
	GE::Ref<GE::SubTexture2D> m_TextureBarrel;

	GE::Ref<GE::VertexArray> m_SquareVA;

	glm::vec3 m_SquadPos = { 0.0f, 0.0f, 0.0f };
	float m_Rotation = 0;

	glm::vec4 m_SquareColor = { 1,0.3,0.4,1.0f };

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;

	uint32_t m_MapHeight, m_MapWidth;


	std::unordered_map<char, GE::Ref<GE::SubTexture2D>> s_TextureMap;
};

