// CKR Engine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Engine.h>

class GameApplication : public GEngine::Application {
public:
	GameApplication() {

	}
	~GameApplication(){

	}
};

GEngine::Application* GEngine::CreateApplication() {
	return new GameApplication();
}



