#pragma once

#include "../Layer.h"
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
		
		virtual void OnRender() override;
		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}
