#pragma once


#ifdef GE_PLATFORM_WINDOW

extern GEngine::Application* GEngine::CreateApplication();


int main(int argc, char** argv)
{
	GEngine::Log::Init();
	GELog_Info("Info Log!");
	GELog_Warn("Warn Log!");
	GELog_Trace("Trace Log! Test {0},Test {1}", 5, 1222.02);
	GELog_Fatal("fatal Log!");
	GELog_Error("error Log!");
	GEngine::Log::GetClientLogger()->critical("Just a prank xd");
	
	auto app = GEngine::CreateApplication();
	app->Run();
	delete app;
}
#endif;
