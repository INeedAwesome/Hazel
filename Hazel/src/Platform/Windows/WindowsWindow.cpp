#include "hzpch.h"
#include "WindowsWindow.h"

namespace Hazel {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props) 
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{ 
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}


	void WindowsWindow::Init(const WindowProps& props)
	{ 
		m_data.Title = props.Title;
		m_data.Width = props.Width;
		m_data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO : implement a way to terminate glfw
			int succes = glfwInit();
			HZ_CORE_ASSERT(succes, "Could not Initialize GLFW!");
			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)props.Width, (int)props.Height, m_data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	void WindowsWindow::WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::OnUpdate()
	{ 
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
		m_data.VSync = enabled;
	}

	bool WindowsWindow::WindowsWindow::IsVsync() const
	{
		return m_data.VSync;
	}
}