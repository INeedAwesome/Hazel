#include "hzpch.h"
#include "DirectX12Context.h"

#include "Platform/Windows/WindowsWindow.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	DirectX12Context::DirectX12Context(HWND windowHandle)
		: m_HWND(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void Hazel::DirectX12Context::Init()
	{

		HRESULT hr;

		DXGI_MODE_DESC bufferDesc = { 0 };
		bufferDesc.Width = 1280;
		bufferDesc.Height = 720;
		bufferDesc.RefreshRate = { 60, 1 };
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = m_HWND;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_D3D11Device, NULL, &m_D3D11DevCon);

		ID3D11Texture2D* backBuffer;
		hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

		hr = m_D3D11Device->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);
		backBuffer->Release();
		m_D3D11DevCon->OMSetRenderTargets(1, &m_RenderTargetView, NULL);

	}

	void Hazel::DirectX12Context::SwapBuffers()
	{
		m_SwapChain->Present(0, 0);

	}
}