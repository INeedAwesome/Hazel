#pragma once

#include "Hazel/Renderer/RendererAPI.h"

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <DirectXMath.h>

#define __cplusplus
#include <dxgi1_6.h>
#include <d3dcompiler.h>

namespace Hazel {

	class DirectX12RendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void Resize(int width, int height) override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

	private:
	};

}

