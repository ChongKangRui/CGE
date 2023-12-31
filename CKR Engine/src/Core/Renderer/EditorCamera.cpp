#include "gepch.h"
#include "EditorCamera.h"
#include "Core/Base/Input.h"
#include "Core/Base/KeyCode.h"
#include "Core/Base/MouseCode.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace GE {



	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearclip, float farclip) : m_FOV(fov), m_AspectRatio(aspectRatio), m_Near(nearclip), m_Far(farclip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearclip, farclip))
	{
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep s)
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;

		if (Input::IsKeyPressed(KEY_LEFT_ALT)) {

			if (Input::IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
				MouseZoom(delta.y);
		}
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			MouseRotate(delta);


		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GE_BEVENT_FN(EditorCamera::OnMouseScroll));
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);

	}

	void EditorCamera::UpdateView()
	{
	//	m_Yaw = m_Pitch = 0;
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;

	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();

		if (Input::IsKeyPressed(KEY_A))
			m_FocalPoint -= (GetRightDirection() * m_CameraMovingSensitivity);
		if (Input::IsKeyPressed(KEY_D))
			m_FocalPoint += (GetRightDirection() * m_CameraMovingSensitivity);
		if (Input::IsKeyPressed(KEY_W))
			m_FocalPoint += (GetForwardDirection() * m_CameraMovingSensitivity);
		if (Input::IsKeyPressed(KEY_S))
			m_FocalPoint -= (GetForwardDirection() * m_CameraMovingSensitivity);
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		//GELog_Trace("Forcal point= {0} ,{1}, {2}||||| Get Forward Direction = {3} ,{4}, {5}", m_FocalPoint.x, m_FocalPoint.y, m_FocalPoint.z, GetForwardDirection().x, GetForwardDirection().y, GetForwardDirection().z);
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;


		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}

}
