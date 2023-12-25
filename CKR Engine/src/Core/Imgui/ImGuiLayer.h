#pragma once

#include "Core/Base/Layer.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/ApplicationEvent.h"


namespace GE {
	class GE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColor();
	private:
		float m_Time = 0.0f;
		bool m_BlockEvents = true;
	};
}
