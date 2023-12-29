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
		m_SelectedEntity = {};
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

		//Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Create Empty Entity")) 
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
			
		}

		ImGui::End();


		ImGui::Begin("Properties");

		if (m_SelectedEntity) {

			DrawComponents(m_SelectedEntity);

			////If button click, open pop up
			//if (ImGui::Button("Add Component")) {
			//	ImGui::OpenPopup("AddComponent");
			//}

			//if (ImGui::BeginPopup("AddComponent")) {


			//	///Temporary, in the future, should do loop iteration
			//	if (ImGui::MenuItem("Camera")) {
			//		m_SelectedEntity.AddComponent<CameraComponent>();
			//		ImGui::CloseCurrentPopup();
			//	}

			//	if (ImGui::MenuItem("SpriteRender")) {
			//		m_SelectedEntity.AddComponent<SpriteComponent>();
			//		ImGui::CloseCurrentPopup();
			//	}


			//	ImGui::EndPopup();
			//}


		}

		ImGui::End();

	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{

		auto& tc = entity.GetComponent<TagComponent>();
		
		//set the visualization of flag being selected
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;


		//provide an unique identify for this treenode, 
		// so we provide an entity id
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tc.Tag.c_str());

		if (ImGui::IsItemClicked()) {
			m_SelectedEntity = entity;

		}

		bool entityDeleted = false;

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity")) {
				entityDeleted = true;
				
			}
			ImGui::EndPopup();
		}

		if (opened) {

			ImGui::TreePop();

		}


		if (entityDeleted) {
			if (m_SelectedEntity == entity)
				m_SelectedEntity = {};
			m_Context->DestroyEntity(entity);
			

		}
			

	}

	static void DrawButton(const std::string& buttonName, float& value,ImVec2& buttonSize, ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonPressColor, float resetValues = 0.0f, ImFont* font = nullptr) {

		if (!font) {
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];
			font = boldFont;
		}



		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoverColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonPressColor);

		ImGui::PushFont(font);
		if (ImGui::Button(buttonName.c_str(), buttonSize)) {
			value = resetValues;
		}
		ImGui::PopFont();
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
		
		DrawButton("x", values.x, buttonSize, ImVec4{ 0.8f,0.1f,0.1f,1.0f }, ImVec4{ 1.0f,0.2f,0.2f,1.0f }, ImVec4{ 0.9f,0.15f,0.15f,1.0f });
		DrawButton("y", values.y, buttonSize, ImVec4{ 0.1f,0.8f,0.2f,1.0f }, ImVec4{ 0.2f,1.0f,0.2f,1.0f }, ImVec4{ 0.1f,0.9f,0.2f,1.0f }, 1.0f);
		DrawButton("z", values.z, buttonSize, ImVec4{ 0.1f,0.1f,0.8f,1.0f }, ImVec4{ 0.2f,0.2f,1.0f,1.0f }, ImVec4{ 0.2f,0.2f,0.9f,1.0f });


		ImGui::PopStyleVar();

		ImGui::Columns(1);


		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction function) {
		
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_Selected;


		if (entity.HasComponent<T>()) {

			
			
			auto& component = entity.GetComponent<T>();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());

			bool removeComponent = false;
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Remove Component")) {
					removeComponent = true;
				}
				ImGui::EndPopup();
			}

			

			ImGui::PopStyleVar();

			

			if (open)
			{
				
				function(component);
				ImGui::TreePop();

			}

			if (removeComponent)
				entity.RemoveComponent<T>();



		}
		

	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{

		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];

			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer,sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
			
				tag = std::string(buffer);
			
			}

		}    

		ImGui::SameLine();
		//ImGui::PushItemWidth(-1);
		//If button click, open pop up
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent")) {


			///Temporary, in the future, should do loop iteration
			if (ImGui::MenuItem("Camera")) {
				m_SelectedEntity.AddComponent<CameraComponent>();
				m_Context->OnViewportResize(m_Context->m_ViewportWidth, m_Context->m_ViewportHeight);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("SpriteRender")) {
				m_SelectedEntity.AddComponent<SpriteComponent>();
				ImGui::CloseCurrentPopup();
			}


			ImGui::EndPopup();
		}

		//ImGui::PopItemWidth();


		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;


		DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {

			

			DrawVec3Control("Position", component.Position);

			//Transform rotation into degree
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);

			DrawVec3Control("Scale", component.Scale, 1.0f);
			
			});

		DrawComponent<CameraComponent>("Camera", entity, [&](auto& component) {
			auto& camera = component.Camera;
			
			const char* projectionTypeStrings[] = { "Perpestive", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			ImGui::Checkbox("Primary", &component.Primary);


			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {

				for (auto currentType : projectionTypeStrings) {
					bool isSelected = currentProjectionTypeString == currentType;
					if (ImGui::Selectable(currentType, isSelected)) {
						currentProjectionTypeString = currentType;
						int i = currentProjectionTypeString == "Perpestive" ? 0 : 1;
						camera.SetProjectionType((SceneCamera::ProjectionType)i);

						m_Context->OnViewportResize(m_Context->m_ViewportWidth, m_Context->m_ViewportHeight);
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

				if (ImGui::DragFloat("Size", &orthosize)) {
					camera.SetOrthographicSize(orthosize);
				}
				if (ImGui::DragFloat("Near Clip", &orthoNear)) {
					camera.SetOrthographicNearClip(orthoNear);
				}
				if (ImGui::DragFloat("Far Clip", &orthoFar)) {
					camera.SetOrthographicFarClip(orthoFar);
				}

				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});

		DrawComponent<SpriteComponent>("Sprite Renderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

			});


		

	}

}