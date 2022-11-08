#pragma once

#include "Hazel/Renderer/GraphicsContext.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#pragma comment(lib,"d3d12.lib")

#ifdef HZ_DEBUG
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

namespace Hazel {

	class DirectX12Context : public GraphicsContext
	{
	public:
		DirectX12Context(HWND hwnd);

		virtual void Init() override;
		virtual void SwapBuffers() override;

		static const int NUM_FRAME_BUFFERS = 3;
		static const int NUM_FRAMES_IN_FLIGHT = 3;
		HWND g_HWND;
		static IDXGISwapChain3* g_pSwapChain;
		static ID3D12Device* g_pD3D12Device;
		static ID3D12CommandQueue* g_pD3D12CommandQueue;
		static ID3D12GraphicsCommandList* g_pD3D12CommandList;
	
		static ID3D12DescriptorHeap* g_pD3DRtvDescHeap;
		static ID3D12DescriptorHeap* g_pd3dSrvDescHeap;
		static D3D12_CPU_DESCRIPTOR_HANDLE g_mainRenderTargetDescriptor[NUM_FRAME_BUFFERS];
	private:


	};

}
