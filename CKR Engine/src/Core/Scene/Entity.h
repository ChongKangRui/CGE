#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace GE {
	class Entity {
	public:
		Entity() = default;

		Entity(entt::entity handle, Scene* scene);
		
		//Copy constructor
		Entity(const Entity& other) = default;


		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.all_of<T>(m_EntityID);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			GE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			return m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() {
			GE_CORE_ASSERT(HasComponent<T>(), "Entity does not has component");

			return m_Scene->m_Registry.get<T>(m_EntityID);
		}

		template<typename T>
		void RemoveComponent() {
			m_Scene->m_Registry.remove<T>(m_EntityID);
		}

		operator bool() const { return m_EntityID != entt::null; }

	private:
		//Represent like an ID to the entity
		//This is just an uint32_t by default definition
		entt::entity m_EntityID{entt::null};

		/// in the future, this raw pointer should replace 
		//  with some sort of weak ptr
		Scene* m_Scene = nullptr;

	};

	




}