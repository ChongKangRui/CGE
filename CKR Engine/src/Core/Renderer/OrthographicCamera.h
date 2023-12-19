#pragma once
#include <glm/glm.hpp>


namespace GE {
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		
		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const {
			return m_Position;
		}

		const float& GetRotation() const {
			return m_Rotation;
		}
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix();
		}
		void SetRotation(const float& rotation) {
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}

		
		const glm::mat4& GetViewProjectionMatrix() const {
			return m_ViewProjectionMatrix;
		}

		const glm::mat4& GetViewMatrix() const {
			return m_ViewMatrix;
		}
		const glm::mat4& GetProjectionMatrix() const {
			return m_ProjectionMatrix;
		}
	private:
		void RecalculateViewMatrix();

	private:
		/*responsible for transforming camera space 
		coordinates to clip space coordinates. It 
		handles perspective effects, such as 
		foreshortening and depth perception. The 
		projection matrix simulates how the 3D 
		scene is projected onto a 2D plane (the screen).*/
		glm::mat4 m_ProjectionMatrix;

		/*The view matrix represents the 
		transformation from world coordinates to 
		camera coordinates. It defines how the 
		world is viewed from the perspective 
		of the camera. In other words, it
		transforms the entire scene to simulate 
		the camera's position, orientation, and
		field of view.*/
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;
	};
}

