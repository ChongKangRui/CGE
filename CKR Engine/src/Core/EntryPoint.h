#pragma once


#ifdef GE_PLATFORM_WINDOW

extern GEngine::Application* GEngine::CreateApplication();


int main(int argc, char** argv)
{
	printf("Engine\n");
	auto app = GEngine::CreateApplication();
	app->Run();
	delete app;
}
#endif;
