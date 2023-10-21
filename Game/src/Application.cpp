// CKR Engine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Engine.h>
#include <Core/EngineApplication.h>

using namespace GE;

class ExampleLayer : public GE::Layer {
public:
	ExampleLayer() : Layer("Testing") {};
	void OnUpdate() override {
		/*if (Input::IsKeyPressed(KEY_A)) {
			KeyPressedEvent& e = (KeyPressedEvent&)eventss;
			Log_Trace("{0}", (e.GetKeyCode()));
		}*/
	}

	void OnEvent(GE::Event& event) override {

		Log_Info("{0}", event);
		if (event.GetEventType() == EventType::KeyPressed) {
			
			KeyPressedEvent& e = (KeyPressedEvent&)event;
			Log_Trace("{0}", (char)e.GetKeyCode());

			if (e.GetKeyCode() == KEY_TAB)
				Log_Trace("Tab Pressed");
				
		}
		
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



