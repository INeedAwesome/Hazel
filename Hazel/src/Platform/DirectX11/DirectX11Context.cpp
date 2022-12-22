#include "hzpch.h"
#include "DirectX11Context.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Hazel/Application.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

namespace Hazel {

	DirectX11Context::DirectX11Context(GLFWwindow* glfwWindow)
	{
		HZ_CORE_ASSERT(glfwWindow, "Window handle is null!");
		m_glfwWindow = glfwWindow;
		m_Hwnd = glfwGetWin32Window(m_glfwWindow);
	}

	void DirectX11Context::Init(const WindowProps& props)
	{
		int width = props.Width;
		int height = props.Height;

		IDXGISwapChain* swapChain = nullptr;
		ID3D11Device* d3d11Device = nullptr;
		ID3D11DeviceContext* d3dDeviceContext = nullptr;
		ID3D11RenderTargetView* renderTargetView = nullptr;

		ID3D11DepthStencilView* depthStencilView;
		ID3D11Texture2D* depthStencilBuffer;

		DXGI_MODE_DESC dxgiModeDesc;
		ZeroMemory(&dxgiModeDesc, sizeof(dxgiModeDesc));
		{
			dxgiModeDesc.Width = width;
			dxgiModeDesc.Height = height;
			dxgiModeDesc.RefreshRate.Numerator = 60;
			dxgiModeDesc.RefreshRate.Denominator = 1;
			dxgiModeDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			dxgiModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			dxgiModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		}

		DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
		ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
		{
			dxgiSwapChainDesc.BufferDesc = dxgiModeDesc;
			dxgiSwapChainDesc.SampleDesc.Count = 1;
			dxgiSwapChainDesc.SampleDesc.Quality = 0;
			dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			dxgiSwapChainDesc.BufferCount = 1;
			dxgiSwapChainDesc.OutputWindow = m_Hwnd;
			dxgiSwapChainDesc.Windowed = true;
			dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		}

		//Describe our Depth/Stencil Buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		{
			depthStencilDesc.Width = width;
			depthStencilDesc.Height = height;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDesc.CPUAccessFlags = 0;
			depthStencilDesc.MiscFlags = 0;
		}

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0
		};


		uint32_t creationFlags = 0;
#ifdef HZ_DEBUG
		// If the project is in a debug build, enable debugging via SDK Layers.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // HZ_DEBUG

		// Creation of the directX 11 Device (gpu) and the Swapchain(consists of front and back buffer)
		HZ_CORE_ASSERT(
		SUCCEEDED(
			D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, featureLevels, ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION, &dxgiSwapChainDesc, &swapChain, &d3d11Device, nullptr, &d3dDeviceContext
			)
		), "Failed to create DirectX 11 device and swapchain!");
		// BASICLY 
		//  if D3D11CreateDeviceAndSwapChain failed HZ_CORE_ASSERT is triggerd. 

		//Create our BackBuffer
		ID3D11Texture2D* backBuffer;
		HZ_CORE_ASSERT(SUCCEEDED(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))), "Failed to create back buffer!");

		//Create our Render Target
		HZ_CORE_ASSERT(SUCCEEDED(d3d11Device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView)), "Failed to create render target view of the back buffer");
		backBuffer->Release();
		
		// create a depth stencil view
		d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
		d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
		
		//Set our Render Target
		d3dDeviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

#ifdef HZ_DEBUG
		// Set up debug layer to break on D3D11 errors
		ID3D11Debug* d3dDebug = nullptr;
		HZ_CORE_ASSERT(SUCCEEDED(d3d11Device->QueryInterface(IID_PPV_ARGS(&d3dDebug))), "Couldn't query the debug interface");
		if (d3dDebug)
		{
			ID3D11InfoQueue* d3dInfoQueue = nullptr;
			if (SUCCEEDED(d3dDebug->QueryInterface(IID_PPV_ARGS(&d3dInfoQueue))))
			{
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
				d3dInfoQueue->Release();
			}
			d3dDebug->Release();
		}
#endif // HZ_DEBUG

#pragma region Set Variables In m_D3DInstance

		m_D3DInstance->GetInstance().m_hwnd = m_Hwnd;
		m_D3DInstance->GetInstance().m_glfwWindow = m_glfwWindow;

		m_D3DInstance->GetInstance().m_SwapChain = swapChain;
		m_D3DInstance->GetInstance().m_d3d11Device = d3d11Device;
		m_D3DInstance->GetInstance().m_d3dDeviceContext = d3dDeviceContext;
		m_D3DInstance->GetInstance().m_renderTargetView = renderTargetView;

		m_D3DInstance->GetInstance().m_depthStencilView = depthStencilView;
		m_D3DInstance->GetInstance().m_depthStencilBuffer = depthStencilBuffer;

#pragma endregion
	}

	void DirectX11Context::SwapBuffers()
	{
		m_D3DInstance->GetInstance().m_SwapChain->Present(1, 0);

		glfwSwapBuffers(m_D3DInstance->GetInstance().m_glfwWindow);
	}
}