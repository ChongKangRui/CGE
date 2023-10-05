#pragma once
#include "Core.h"

namespace GEngine {


	class GE_API Application
	{
	public:
		Application();

		//will be inherited from game aplication
		virtual ~Application();
		//Run our application
		void Run();

	};
	// To be define in the client
	Application* CreateApplication();

}

