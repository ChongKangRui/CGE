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

	private:
		//An id that for the entity
		//represent the id that belong to component as well
		entt::registry m_Registry;

		friend class Entity;
	};
}
