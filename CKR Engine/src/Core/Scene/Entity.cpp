#include "gepch.h"
#include "Entity.h"

namespace GE {
	Entity::Entity(entt::entity handle, Scene* scene) : m_EntityID(handle), m_Scene(scene)
	{

	}
}
