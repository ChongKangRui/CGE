#include "gepch.h"
#include "OrthographicCameraController.h"
#include "Core/Input.h"
#include "Core/KeyCode.h"


namespace GE {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotationControl) : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio*m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotationControl)
	{


	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
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

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return true;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth()/ (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return true;
	}

}