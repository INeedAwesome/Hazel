#pragma once

#include "Hazel/Renderer/RendererAPI.h"

#include "Platform/DirectX12/DirectX12Context.h"

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

