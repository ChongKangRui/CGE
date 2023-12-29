#pragma once
#include <glm/glm.hpp>

#include "Camera.h"
#include "Core/Base/TimeStep.h"
#include "Core/Event/Event.h"
#include "Core/Event/MouseEvent.h"

namespace GE {
	class EditorCamera : public Camera
	{
	public: 

		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearclip, float farclip);

		void OnUpdate(Timestep s);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewportSize(float w, float h) { m_ViewportWidth = w; m_ViewportHeight = h; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3 GetPosition() const { return m_Position; };
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;

		float RotationSpeed() const;
		float ZoomSpeed() const;

		float m_FOV = 45.0f;
		float m_Near = 0.1f, m_Far = 1000.0f;
		float m_AspectRatio = 1.778f;

		float m_Yaw = 0.0f, m_Pitch = 0.0f;
		
		float m_Distance = 100.0f;
		float m_ViewportWidth = 1280, m_ViewportHeight = 720;

		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_MovementPos = { 0.0f, 0.0f, 0.0f };
		glm::mat4 m_ViewMatrix;
		glm::vec2 m_InitialMousePosition;

		//Sensitivity of wasd moving camera
		float m_CameraMovingSensitivity = 0.1f;
	};

}

