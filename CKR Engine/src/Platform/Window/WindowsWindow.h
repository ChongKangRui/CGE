#pragma once

#include "Core/Window.h"

#include "Core/Log.h"

#include <glfw/glfw3.h>
#include "Core/Renderer/GraphicContext.h"

namespace GE {
	
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		//Bind the event on the beginning
		inline void SetEventCallback(const EventCallbackFn& callback) override {

			m_Data.EventCallback = callback;
		}

		inline virtual void* GetNativeWindow() const {return m_Window;}

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

