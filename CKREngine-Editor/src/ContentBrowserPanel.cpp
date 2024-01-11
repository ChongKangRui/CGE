#include "gepch.h"
#include "ContentBrowserPanel.h"

#include <Imgui/imgui.h>

namespace GE {

	// once we have projects, change this
	extern const std::filesystem::path s_AssetsDirectory = "assets";

	ContentBrowserPanel::ContentBrowserPanel() : m_CurrentDirectory(s_AssetsDirectory)
	{
		m_FolderIcon = Texture2D::Create("Resources/Icon/Folder.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Btowser");

		if (m_CurrentDirectory != std::filesystem::path(s_AssetsDirectory)) {

			if (ImGui::Button("<-")) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		int columnCount = (int)ImGui::GetContentRegionAvail().x;
		ImGui::Columns(columnCount / (m_Padding + m_ThumbnailSize),0,false);

		
		//Iterate directory
		for (auto it : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			//Need to push id because id for all image button is identical which is same image for now
			// so we need to specify id in order to make sure imgui recognize which item we are dragging
			
			std::string path = it.path().string();
			auto relativePath = std::filesystem::relative(it.path(), s_AssetsDirectory);

			auto test = relativePath.filename().string().c_str();

			ImGui::PushID(relativePath.filename().string().c_str());
			//File name only
			std::string relativePathString = relativePath.filename().string();

#pragma region Debug
			/*auto p1 = p.path().filename().string();
			auto p2 = p.path().stem().string();*/
			//stem return a name without extention, like test.txt without .txt
			//ImGui::Text("filename %s", p1.c_str());
			//ImGui::Text("stem %s", p2.c_str());

#pragma endregion
			//Push and pop disable the grey background of the button
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			ImGui::ImageButton((ImTextureID)m_FolderIcon->GetRendererID(), { m_ThumbnailSize,m_ThumbnailSize }, { 0,1 }, { 1,0 });

			if (ImGui::BeginDragDropSource()) {
				auto itemPath = relativePath.c_str();
				//+1 to access null termination character
				ImGui::SetDragDropPayload("Content_Browser_Item", itemPath, (wcslen(itemPath)+1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (it.is_directory()) {
					//std::string p = path.path().string();
					m_CurrentDirectory /= it.path().filename();
				}
			}
			ImGui::TextWrapped(relativePathString.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}


		ImGui::End();
	}

}
