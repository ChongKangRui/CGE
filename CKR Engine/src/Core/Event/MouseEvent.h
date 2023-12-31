#pragma once
#include "gepch.h"
#include "Event.h"




namespace GE {

	class GE_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {

		}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_GATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class GE_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xoffset, float yoffset) : m_XOffset(xoffset), m_YOffset(yoffset) {

		}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_GATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};

	class GE_API MouseButtonEvent : public Event {
	public:


		inline int GetMouseButton() const { return m_Button; }
		
		EVENT_CLASS_GATEGORY(EventCategoryMouse | EventCategoryInput)
			
	protected:
		MouseButtonEvent(int button) : m_Button(button) {}
		int m_Button;
	};

	class GE_API MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
			EVENT_CLASS_GATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class GE_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
			EVENT_CLASS_GATEGORY(EventCategoryMouse | EventCategoryInput)
	};


}