#pragma once
#include "Core/Core.h"

//! This is an entry point which will be used to serve as program entry point
#ifdef GE_PLATFORM_WINDOW

extern GE::Application* GE::CreateApplication();


int main(int argc, char** argv)
{
	GE::Log::Init();
	GELog_Info("Initialize application!");
	GE_PROFILE_BEGIN_SESSION("StartUp", "GEProfile-Startup.json");
	auto* app = GE::CreateApplication();
	GE_PROFILE_END_SESSION();

	GE_PROFILE_BEGIN_SESSION("Runtime", "GEProfile-Runtime.json");
	app->Run();
	GE_PROFILE_END_SESSION();

	GE_PROFILE_BEGIN_SESSION("End", "GEProfile-End.json");
	delete app;
	GE_PROFILE_END_SESSION();

}
#endif;
