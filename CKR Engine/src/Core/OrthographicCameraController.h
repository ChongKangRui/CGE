#pragma once
#include "Renderer/OrthographicCamera.h"
#include "Core/Base/TimeStep.h"

#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/MouseEvent.h"


namespace GE {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};



	class OrthographicCameraController
	{
	public:
		// start at 2 units in space
		// aspectratio * 2 units
		OrthographicCameraController(float aspectRatio, bool rotationControl = false);
		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView();
		}

		OrthographicCamera& GetCamera(){ return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	

	private:
		//the order of initialization
		bool  m_Rotation;
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;
		

		float m_CameraRotation = 0.0f;
		glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};

		float m_CameraMoveSpeed = 1.0f;
		float m_CameraRotationSpeed = 1.0f;

		


	};
}
