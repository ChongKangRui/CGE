#pragma once
#include <glm/glm.hpp>

#include "Core/Renderer/Camera.h"

namespace GE {
	/*will be a representation UI in the future for like the ui property*/
	class SceneCamera : public Camera {

	public:
		SceneCamera();
		virtual ~SceneCamera();
		
		void SetOrthographic(float size, float nearclip, float farclip);
		
		void SetViewportSize(uint32_t width, uint32_t height);


		void SetOrthographicSize(float size) {
			m_OrthographicSize = size; 
			RecalculateProjection();
		};

		float GetOrthographicSize(){
			return m_OrthographicSize;
		}
	private:
		void RecalculateProjection();
	
	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};


}