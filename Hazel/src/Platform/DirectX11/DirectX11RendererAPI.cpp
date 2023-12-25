#include "hzpch.h"
#include "DirectX11RendererAPI.h"
#include "DirectX11Context.h"

namespace Hazel {

	void DirectX11RendererAPI::SetClearColor(const glm::vec4& color)
	{
		float bgColor[] = { color.r, color.g, color.b, color.a };
		m_D3DInstance->GetInstance().m_d3dDeviceContext->ClearRenderTargetView(m_D3DInstance->GetInstance().m_renderTargetView, bgColor);
	}

	void DirectX11RendererAPI::Clear()
	{
		m_D3DInstance->GetInstance().m_d3dDeviceContext->ClearDepthStencilView( m_D3DInstance->GetInstance().m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DirectX11RendererAPI::Resize(int width, int height)
	{
	}

	void DirectX11RendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
	}
}