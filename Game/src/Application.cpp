// CKR Engine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Engine.h>
#include <Core/EngineApplication.h>

class ExampleLayer : public GE::Layer {
public:
	ExampleLayer() : Layer("Testing") {};
	void OnUpdate() override {
		//Log_Info("Testing layer here");
	}

	void OnEvent(GE::Event& event) override {

		Log_Info("{0}", event);
		
	}
};


class GameApplication : public GE::Application {
public:
	GameApplication() {
		PushLayer(new ExampleLayer());
		PushOverlay(new GE::ImGuiLayer());
	}
	~GameApplication(){

	}

};

GE::Application* GE::CreateApplication() {
	return new GameApplication();
}



