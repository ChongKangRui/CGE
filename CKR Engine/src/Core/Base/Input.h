#pragma once

#include "Core/Core.h"
namespace GE {
	//like a interface
	class GE_API Input {
	public:
		 static bool IsKeyPressed(int keycode);
		 static bool IsMouseButtonPressed(int keycode);
		 static float GetMouseX();
		 static float GetMouseY();
		 static std::pair<float, float> GetMousePosition();
	};
}