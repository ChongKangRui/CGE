#include <glm/gtc/type_ptr.hpp>
#include <Imgui/imgui.h>
#include "SceneHierarchyPanel.h"
#include "Core/Scene/Component.h"


namespace GE {



	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID) {

			Entity entity{ entityID , m_Context.get() };
			DrawEntityNode(entity);

			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_SelectedEntity = {};
		}


		ImGui::End();


		ImGui::Begin("Properties");

		if(m_SelectedEntity)
			DrawComponents(m_SelectedEntity);

		ImGui::End();

	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tc = entity.GetComponent<TagComponent>();
		
		

		//set the visualization of flag being selected
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		
		//provide an unique identify for this treenode, 
		// so we provide an entity id
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tc.Tag.c_str());

		if (ImGui::IsItemClicked()) {
			m_SelectedEntity = entity;

		}

		if (opened) {

			ImGui::TreePop();

		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{

		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];

			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer,sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
			
				tag = std::string(buffer);
			
			}

		}



		if (entity.HasComponent<TransformComponent>()) {

			auto& transform = entity.GetComponent<TransformComponent>().Transform;

			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				if (ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f)) {

				}
				ImGui::TreePop();
			}


		}

		if (entity.HasComponent<CameraComponent>()) {

			

			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				
				auto& camera = entity.GetComponent<CameraComponent>().Camera;
				auto& cameraComp = entity.GetComponent<CameraComponent>();

				const char* projectionTypeStrings[] = { "Perpestive", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				
				ImGui::Checkbox("Primary", &cameraComp.Primary);

				
				if (ImGui::BeginCombo("Projection ", currentProjectionTypeString)) {

					for (auto currentType : projectionTypeStrings) {
						bool isSelected = currentProjectionTypeString == currentType;
						if (ImGui::Selectable(currentType, isSelected)) {
							currentProjectionTypeString = currentType;

							int i = currentProjectionTypeString == "Perpestive" ? 0 : 1;
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}


					ImGui::EndCombo();
				}
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
					float perspectiveFOV = camera.GetPerspectiveFOV();
					float perspectiveNear = camera.GetPerspectiveNearClip();
					float perspectiveFar = camera.GetPerspectiveFarClip();

					if (ImGui::DragFloat("FOV", &perspectiveFOV)) {
						camera.SetPerspectiveFOV(perspectiveFOV);
					}
					if (ImGui::DragFloat("Near Clip", &perspectiveNear)) {
						camera.SetPerspectiveNearClip(perspectiveNear);
					}
					if (ImGui::DragFloat("Far Clip", &perspectiveFar)) {
						camera.SetPerspectiveFarClip(perspectiveFar);
					}
				}
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
					
					float orthosize = camera.GetOrthographicSize();
					float orthoNear = camera.GetOrthographicNearClip();
					float orthoFar = camera.GetOrthographicFarClip();

					if(ImGui::DragFloat("Size", &orthosize)){
						camera.SetOrthographicSize(orthosize);
					}
					if (ImGui::DragFloat("Near Clip", &orthoNear)) {
						camera.SetOrthographicNearClip(orthoNear);
					}
					if (ImGui::DragFloat("Far Clip", &orthoFar)) {
						camera.SetOrthographicFarClip(orthoFar);
					}
					
					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComp.FixedAspectRatio);
				}




				ImGui::TreePop();

			}


		}

	}

}