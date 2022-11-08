#include "hzpch.h"
#include "DirectX12Context.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Hazel {

	DirectX12Context::DirectX12Context(HWND windowHandle)
		: g_HWND(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void Hazel::DirectX12Context::Init()
	{
		DXGI_SWAP_CHAIN_DESC1 scd;
		{
			ZeroMemory(&scd, sizeof(scd));
			scd.Width = 0;
			scd.Height = 0;
			scd.Format = DXGI_FORMAT_B5G6R5_UNORM;
			scd.Stereo = false;
			scd.SampleDesc.Count = 1;
			scd.SampleDesc.Quality = 0;
			scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scd.BufferCount = NUM_FRAME_BUFFERS;
			scd.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;
			scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			scd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			scd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
		}

#if HZ_DEBUG
		ID3D12Debug* pDx12Debug = NULL;
		if (D3D12GetDebugInterface(IID_PPV_ARGS(&pDx12Debug)))
		{
			pDx12Debug->EnableDebugLayer();
		}

		if (pDx12Debug)
		{
			ID3D12InfoQueue* pInfoQueue = NULL;
			g_pD3D12Device->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
			pInfoQueue->Release();
			pDx12Debug->Release();
		}
#endif // HZ_DEBUG

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_0;
		HZ_CORE_ASSERT(D3D12CreateDevice(NULL, featureLevel, IID_PPV_ARGS(&g_pD3D12Device)), "Could not create DirectX12 device!");

		D3D12_DESCRIPTOR_HEAP_DESC dhdesc;
		{
			ZeroMemory(&dhdesc, sizeof(dhdesc));
			dhdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			dhdesc.NumDescriptors = NUM_FRAME_BUFFERS;
			dhdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			dhdesc.NodeMask = 1;
		}
		HZ_CORE_ASSERT(g_pD3D12Device->CreateDescriptorHeap(&dhdesc, IID_PPV_ARGS(&g_pD3DRtvDescHeap)), "Could not create descriptor heap type: D3D12_DESCRIPTOR_HEAP_TYPE_RTV!");

		unsigned __int64 rtvDescSize = g_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pD3DRtvDescHeap->GetCPUDescriptorHandleForHeapStart();

		for (int i = 0; i < NUM_FRAME_BUFFERS; i++)
		{
			g_mainRenderTargetDescriptor[i] = rtvHandle;
			rtvHandle.ptr += rtvDescSize;
		}

		// https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_descriptor_heap_type
		D3D12_DESCRIPTOR_HEAP_DESC dhDesc1 = {};
		{
			// The descriptor heap for the combination of constant-buffer, shader-resource, and unordered-access views.
			dhDesc1.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			dhDesc1.NumDescriptors = 1;
			dhDesc1.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		}
		HZ_CORE_ASSERT(g_pD3D12Device->CreateDescriptorHeap(&dhDesc1, IID_PPV_ARGS(&g_pd3dSrvDescHeap)), "Could not create descriptor heap type: D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV!");
	

		D3D12_COMMAND_QUEUE_DESC cqdesc = {};
		{
			cqdesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			cqdesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
			cqdesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			cqdesc.NodeMask = 1;
		}
		HZ_CORE_ASSERT(g_pD3D12Device->CreateCommandQueue(&cqdesc, IID_PPV_ARGS(&g_pD3D12CommandQueue)), "Could not create DirectX12 command queue!");


		for (int i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
			HZ_CORE_ASSERT(g_pD3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator)), "");


	}

	void Hazel::DirectX12Context::SwapBuffers()
	{

	}
}