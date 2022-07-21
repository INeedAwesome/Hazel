#pragma once

#include "Hazel/Window.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Hazel {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		void OnUpdate() override;
		inline unsigned int GetWidth() const override { return m_data.Width; };
		inline unsigned int GetHeight() const override { return m_data.Height; };

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_data.EventCallback = callback; };
		void SetVSync(bool enabled) override;
		bool IsVsync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_window;
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_data;
	};
}

