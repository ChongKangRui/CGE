#pragma once


//! This is an entry point which will be used to serve as program entry point
#ifdef GE_PLATFORM_WINDOW

extern GE::Application* GE::CreateApplication();


int main(int argc, char** argv)
{
	GE::Log::Init();
	GELog_Info("Info Log!");
	GELog_Warn("Warn Log!");
	//GELog_Trace("Trace Log! Test {0},Test {1}", 5, 1222.02);
	GELog_Fatal("fatal Log!");
	GELog_Error("error Log!");
	
	

	auto* app = GE::CreateApplication();
	app->Run();
	delete app;
}
#endif;
