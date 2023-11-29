#pragma once


//! This is an entry point which will be used to serve as program entry point
#ifdef GE_PLATFORM_WINDOW

extern GE::Application* GE::CreateApplication();


int main(int argc, char** argv)
{
	GE::Log::Init();
	GELog_Info("Initialize application!");
	
	auto* app = GE::CreateApplication();
	app->Run();
	delete app;
}
#endif;
