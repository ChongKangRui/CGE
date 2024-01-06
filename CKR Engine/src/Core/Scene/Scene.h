#pragma once
#include "entt.hpp"
#include "Core/Base/TimeStep.h"
#include "Core/Renderer/EditorCamera.h"


namespace GE {

	class Entity; 
	class Scene {

		

	public:
		Scene();
		~Scene();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera cam);
		//Temp 
		entt::registry& Reg() { return m_Registry; }

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);


		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();
	private:
		//Like a manager who responsible to manager all of the entities, components and related stuff.
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class SceneSerializer;
		friend class Entity;
		friend class SceneHierarchyPanel;
		//friend class EditorLayer;
	};

}
