#include <glm/gtc/type_ptr.hpp>
#include <Imgui/imgui.h>
#include <imgui/imgui_internal.h>


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

	static void DrawButton(const std::string& buttonName, float& value,ImVec2& buttonSize, ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonPressColor, float resetValues = 0.0f) {

		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoverColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonPressColor);

		if (ImGui::Button(buttonName.c_str(), buttonSize)) {
			value = resetValues;
		}

		ImGui::PopStyleColor(3);

		//ImGui::SameLine(); instructs ImGui to place the next widget on the same line as the previous one.
		

		std::string name = "##" + buttonName;
		const char* charName = name.c_str();

		ImGui::SameLine();
		ImGui::DragFloat(charName, &value, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
	}

	//our own widget
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValues = 0.0f, float columnWidth = 100.0f) {
		
		//make each row vec3 being individual so each transform, scale and rotation value wont tie together
		ImGui::PushID(label.c_str());
		
		//Initialize 2 column
		ImGui::Columns(2);

		//Set the first column with column width
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Text(label.c_str());

		//Move the column reference into next column
		ImGui::NextColumn();

		/*ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth()); is used to push the widths of multiple items. 
		In this case, it pushes widths for three items. This is often used in conjunction with PopItemWidth().*/
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		//Spacing between each items(vector slider)
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		//Calculate the height based on imgui font size
		//Set the button size of the button that beside the slider button
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f , lineHeight };


		std::unordered_map<std::string, ImVec4> map = { {"X",ImVec4{ 0.8f,0.1f,0.15f,1.0f } } };

		
		/*DrawButton("x", values.x, buttonSize, ImVec4{ 0.1f,0.7f,0.15f,1.0f }, ImVec4{ 1.0f,0.2f,0.2f,1.0f }, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		DrawButton("y", values.y, buttonSize, ImVec4{ 0.8f,0.1f,0.15f,1.0f }, ImVec4{ 0.2f,0.9f,0.2f,1.0f }, ImVec4{ 0.1f,0.8f,0.15f,1.0f });
		DrawButton("z", values.z, buttonSize, ImVec4{ 0.1f,0.2f,0.7f,1.0f }, ImVec4{ 0.2f,0.1f,0.9f,1.0f }, ImVec4{ 0.1f,0.2f,0.8f,1.0f }, 1.0f);*/

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.2f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValues;

		ImGui::PopStyleColor(3);

		//ImGui::SameLine(); instructs ImGui to place the next widget on the same line as the previous one.
		ImGui::SameLine();
		ImGui::DragFloat("##x", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.7f,0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.9f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.8f,0.15f,1.0f });

		if (ImGui::Button("Y", buttonSize))
			values.y = resetValues;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.2f,0.7f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.1f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.2f,0.8f,1.0f });

		if (ImGui::Button("Z", buttonSize))
			values.z = resetValues;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##z", &values.z, 0.1f,0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);


		ImGui::PopID();
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

			
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				DrawVec3Control("Position", transform.Position);

				//Transform rotation into degree
				glm::vec3 rotation = glm::degrees(transform.Rotation);
				DrawVec3Control("Rotation", rotation);
				transform.Rotation = glm::radians(rotation);

				DrawVec3Control("Scale", transform.Scale, 1.0f);
				//ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);

				
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