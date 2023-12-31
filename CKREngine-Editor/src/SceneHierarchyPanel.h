#pragma once

#include "Core/Core.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Entity.h"

namespace GE{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		void SetSelectedEntity(Entity e);

		Entity GetSelectedEntity() const {
			return m_SelectedEntity;
		}

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;

		Entity m_SelectedEntity;

		friend class Scene;
	
	};

}

