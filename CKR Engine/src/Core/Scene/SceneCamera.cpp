#include "gepch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>
namespace GE {
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}
	SceneCamera::~SceneCamera()
	{
	}
	void SceneCamera::SetOrthographic(float size, float nearclip, float farclip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearclip;
		m_OrthographicFar = farclip;

		RecalculateProjection();
	}
	void SceneCamera::SetPerspective(float verticalFOV, float nearclip, float farclip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearclip;
		m_PerspectiveFar = farclip;


		RecalculateProjection();
	}
	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;

		RecalculateProjection();


	}
	void SceneCamera::RecalculateProjection()
	{
		switch (m_ProjectionType) {
		case ProjectionType::Perspective: {
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		break;
		case ProjectionType::Orthographic: {
			float ortholeft = m_AspectRatio * -m_OrthographicSize * 0.5f;
			float orthoRight = m_AspectRatio * m_OrthographicSize * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(ortholeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);

		}
		break;

		}
		
	}
}
