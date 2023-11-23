#pragma once

#include "Core/Core.h"
#include "Event/Event.h"
#include "TimeStep.h"

namespace GE {
	class GE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(TimeStep deltatime) {};
		virtual void OnRender() {};
		virtual void OnEvent(Event& event) {};

		inline const std::string& GetName() const { return m_DebugName; }


	private:
		std::string m_DebugName;


	};
}

