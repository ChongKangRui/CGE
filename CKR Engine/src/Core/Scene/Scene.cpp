#include "gepch.h"
#include "Scene.h"
#include <glm/glm.hpp>

#include "Core/Renderer/Renderer2D.h"

#include "Component.h"
#include "Entity.h"


namespace GE {
	

	Scene::Scene()
	{		


	}

	Scene::~Scene()
	{


	}

	Entity Scene::GetPrimaryCameraEntity() {
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{entity, this};

		}
	}

	void Scene::OnUpdate(Timestep ts)
	{
		/////Update Scripts

		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc){
				
				// Todo: Move to Scene::OnScenePlay
				
				if (!nsc.Instance) {

					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity , this};

					nsc.Instance->OnCreate();

				}
				nsc.Instance->OnUpdate(ts);

				});


		}




		Camera* mainCamera = nullptr;
		glm::mat4 camtransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary) {
					mainCamera = &camera.Camera;
					camtransform = transform.GetTransform();
					break;
				}
			}
		}


		if (mainCamera) {
			
			Renderer2D::BeginScene(mainCamera->GetProjection(), camtransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);

			for (auto gr : group) {

				auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(gr);
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);

			}

			//GE::Renderer2D::DrawQuad({ 0, 0 }, { 1.0f, 2.0f }, 0.0f, { 1.0f,0.3f,1.0f,1.0f });
			Renderer2D::EndScene();

		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);

			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}

	}


	

}

