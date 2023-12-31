#pragma once
#include <glm/glm.hpp>

#include "Core/Renderer/Camera.h"

namespace GE {
	/*will be a representation UI in the future for like the ui property*/
	class SceneCamera : public Camera {

	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };


	public:
		SceneCamera();
		virtual ~SceneCamera();
		
		void SetOrthographic(float size, float nearclip, float farclip);
		void SetPerspective(float verticalFOV, float nearclip, float farclip);
		
		void SetViewportSize(uint32_t width, uint32_t height);

		
		float GetPerspectiveFOV() {return m_PerspectiveFOV;}
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); };
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }


		void SetOrthographicSize(float size) {m_OrthographicSize = size; RecalculateProjection();};
		float GetOrthographicSize(){return m_OrthographicSize;}
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicNearClip(float nearClip)  { m_OrthographicNear = nearClip; RecalculateProjection();}
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection();}



		ProjectionType GetProjectionType() const {
			return m_ProjectionType;
		}

		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }

	private:
		void RecalculateProjection();
	
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		//Orthographic setting
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		//Perpestive setting
		float m_PerspectiveFOV = glm::radians(100.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;




		float m_AspectRatio = 0.0f;
	};


}