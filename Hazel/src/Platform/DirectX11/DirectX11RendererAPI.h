#pragma once

#include "Hazel/Renderer/RendererAPI.h"

#include "Platform/DirectX11/DirectX11Context.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace Hazel {

	class DirectX11RendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void Resize(int width, int height) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	private:
		DirectX11HeaderInstance* m_D3DInstance = &DirectX11HeaderInstance::GetInstance();
	};

}

