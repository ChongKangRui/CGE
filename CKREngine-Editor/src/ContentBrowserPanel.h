#pragma once

#include <filesystem>
#include "Core/Renderer/Texture.h"

namespace GE {
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		inline void SetPadding(float padding) {
			m_Padding = padding;
		};
		inline void SetThumbnailSize(float size){
			m_ThumbnailSize = size;
		};

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_AssetIcon;

		float m_ThumbnailSize = 50.0f;
		float m_Padding = 25.0f;
	};
}

