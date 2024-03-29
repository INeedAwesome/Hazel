#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#ifdef HZ_DIRECTX11
	#include "backends/imgui_impl_dx11.h"
	#include "backends/imgui_impl_win32.h"
	
	#include "Platform/DirectX11/DirectX11HeaderInstance.h"
#endif // HZ_DIRECTX11

#ifdef HZ_OPENGL
	#include "backends/imgui_impl_glfw.h"
	#include "backends/imgui_impl_opengl3.h"
#endif // HZ_OPENGL

#include "Hazel/Core/Application.h"

//temporary
#include <GLFW/glfw3.h>
#include <Glad/glad.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace Hazel {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		HWND hwnd = glfwGetWin32Window(window);

		// Setup Platform/Renderer bindings
#ifdef HZ_DIRECTX11
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(m_D3DInstance->GetInstance().m_d3d11Device, m_D3DInstance->GetInstance().m_d3dDeviceContext);
#endif // HZ_DIRECTX12
#ifdef HZ_OPENGL
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
#endif // HZ_OPENGL

		ImGui::LoadIniSettingsFromDisk("imgui.ini");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui::SaveIniSettingsToDisk("imgui.ini");

#ifdef HZ_DIRECTX11
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
#endif // HZ_DIRECTX12

#ifdef HZ_OPENGL
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
#endif // HZ_OPENGL

		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{

#ifdef HZ_DIRECTX11
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
#endif // HZ_DIRECTX11

#ifdef HZ_OPENGL
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
#endif // HZ_OPENGL
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
#ifdef HZ_DIRECTX11
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif // HZ_DIRECTX11
#ifdef HZ_OPENGL
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // HZ_OPENGL

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		
	}

}