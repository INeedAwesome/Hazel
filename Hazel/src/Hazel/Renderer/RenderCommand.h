#pragma once

#include "RendererAPI.h"

namespace Hazel {

	class RenderCommand 
	{
	public:

		inline static void Init()
		{
			s_RendererAPI->Init();
		}

#pragma region clear

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		
		inline static void Clear() 
		{
			s_RendererAPI->Clear();
		}

#pragma endregion

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

#pragma region draw

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

#pragma endregion


	private:
		static RendererAPI* s_RendererAPI;

	};


}