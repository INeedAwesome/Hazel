#pragma once

#include "Hazel/Renderer/GraphicsContext.h"

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <DirectXMath.h>

#define __cplusplus
#include <dxgi1_6.h>
#include <d3dcompiler.h>

namespace Hazel {

	class DirectX12Context : public GraphicsContext
	{
	public:
		DirectX12Context(HWND hwnd);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		HWND m_HWND;

		IDXGISwapChain* m_SwapChain;
		ID3D11Device* m_D3D11Device;
		ID3D11DeviceContext* m_D3D11DevCon;
		ID3D11RenderTargetView* m_RenderTargetView;
	};

}
