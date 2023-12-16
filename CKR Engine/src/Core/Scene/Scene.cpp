#include "gepch.h"
#include "Scene.h"
#include <glm/glm.hpp>

#include "Core/Renderer/Renderer2D.h"
#include "Component.h"

#include "Entity.h"


namespace GE {
	

	Scene::Scene()
	{

		

		//TransformComponent transform;

		////DoMath(transform);

		////This is actually an uint32_t eventually
		//entt::entity entity = m_Registry.create();
		//m_Registry.emplace<TransformComponent>(entity,glm::mat4(1.0f));
		////m_Registry.emplace<CameraComponent>(entity, 15);

		////m_Registry.remove<TransformComponent>(entity);


		//auto view = m_Registry.view<TransformComponent>();
		//for (auto entity : view) {

		//}


	}

	Scene::~Scene()
	{


	}

	void Scene::OnUpdate(TimeStep ts)
	{

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);

		for (auto gr : group) {
			auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(gr);

			Renderer2D::DrawQuad(transform, sprite.Color);
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

}

