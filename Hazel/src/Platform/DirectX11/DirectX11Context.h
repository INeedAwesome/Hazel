#pragma once

#include "Hazel/Renderer/GraphicsContext.h"
#include "DirectX11HeaderInstance.h"

namespace Hazel {

	class DirectX11Context : public GraphicsContext
	{
	public:
		DirectX11Context(GLFWwindow* glfwWindow);

		virtual void Init(const WindowProps& props) override;
		virtual void SwapBuffers() override;


	private:
		DirectX11HeaderInstance* m_D3DInstance = &DirectX11HeaderInstance::GetInstance();
		HWND m_Hwnd = nullptr;
		GLFWwindow* m_glfwWindow = nullptr;

	};

}
