#pragma once
#include "entt.hpp"
#include "Core/Base/TimeStep.h"


namespace GE {

	class Entity; 
	class Scene {

		

	public:
		Scene();
		~Scene();

		void OnUpdate(TimeStep ts);

		//Temp 
		entt::registry& Reg() { return m_Registry; }

		Entity CreateEntity(const std::string& name = std::string());

		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		//Like a manager who responsible to manager all of the entities, components and related stuff.
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
	};
}
