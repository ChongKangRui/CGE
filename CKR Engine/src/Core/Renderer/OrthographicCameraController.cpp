#include "gepch.h"
#include "OrthographicCameraController.h"
#include "Core/Base/Input.h"
#include "Core/Base/KeyCode.h"


namespace GE {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotationControl) : m_AspectRatio(aspectRatio), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), m_Rotation(rotationControl)
	{


	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		GE_PROFILE_FUNCTION();

		
			if (GE::Input::IsKeyPressed(KEY_A))
				m_CameraPosition.x -= m_CameraMoveSpeed * ts;
			else if (GE::Input::IsKeyPressed(KEY_D))
				m_CameraPosition.x += m_CameraMoveSpeed * ts;


			if (Input::IsKeyPressed(KEY_W))
				m_CameraPosition.y += m_CameraMoveSpeed * ts;
			else if (Input::IsKeyPressed(KEY_S))
				m_CameraPosition.y -= m_CameraMoveSpeed * ts;


			if (m_Rotation) {
				if (Input::IsKeyPressed(KEY_Q))
					m_CameraRotation += m_CameraRotationSpeed * ts;
				if (Input::IsKeyPressed(KEY_E))
					m_CameraRotation -= m_CameraRotationSpeed * ts;

				m_Camera.SetRotation(m_CameraRotation);
			}

			m_Camera.SetPosition(m_CameraPosition);

			m_CameraMoveSpeed = m_ZoomLevel;
		
		//Log_Info("zoom level {0}", m_ZoomLevel);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GE_BEVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GE_BEVENT_FN(OrthographicCameraController::OnWindowResized));

	}
	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;

		CalculateView();
	}


	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };

		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		GE_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		CalculateView();

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		GE_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		
		return false;
	}

	

}