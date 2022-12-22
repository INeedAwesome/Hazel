#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <GLFW/glfw3.h>

namespace Hazel {

	class DirectX11HeaderInstance
	{
	public:

		DirectX11HeaderInstance() 
		{
			DirectX11HeaderInstance::s_instance = this;
		}                    // Constructor? (the {} brackets) are needed here.

		inline static DirectX11HeaderInstance& GetInstance() 
		{ 
			if (!s_instance)
			{
				DirectX11HeaderInstance::s_instance = new DirectX11HeaderInstance();
			}
			return *s_instance;
		} // this is where I get 0x00 reference from


		static const int NUM_FRAME_BUFFERS = 3;
		static const int NUM_FRAMES_IN_FLIGHT = 3;

		D3D11_VIEWPORT m_viewport = {};
		GLFWwindow* m_glfwWindow = nullptr;
		HWND m_hwnd = nullptr;

	
#pragma region initialization
		// Rendering
		ID3D11Device* m_d3d11Device = nullptr;
		IDXGISwapChain* m_SwapChain = nullptr;
		ID3D11DeviceContext* m_d3dDeviceContext = nullptr;
		ID3D11RenderTargetView* m_renderTargetView = nullptr;
		
		// Depth stencil
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11Texture2D* m_depthStencilBuffer;
#pragma endregion

#pragma region assets 
		ID3D11Buffer* m_triangleVertBuffer = nullptr;
		ID3D11VertexShader* m_vertexShader = nullptr;
		ID3D11PixelShader* m_pixelShader = nullptr;
		ID3D10Blob* m_vs_Buffer = nullptr;
		ID3D10Blob* m_ps_Buffer = nullptr;
		ID3D11InputLayout* m_InputLayout = nullptr;
#pragma endregion
		ID3D11RasterizerState* m_rasterizerState = nullptr;


	private:
		static DirectX11HeaderInstance* s_instance;
	};

}