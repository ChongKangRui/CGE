// CKR Engine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Engine.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include "EditorLayer.h"


#include "imgui/imgui.h"

// -------EntryPoint---------------
#include <Core/Base/EntryPoint.h>
//----------------------------------



namespace GE {
	class Editor : public Application {
	public:
		Editor() : Application("GE Editor") {
			PushLayer(new EditorLayer());
			//PushOverlay(new Map_Layer());
		}
		~Editor() {

		}

	};

	GE::Application* GE::CreateApplication() {
		return new Editor();
	}
}



