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

		inline static void Resize(int width, int height)
		{
			s_RendererAPI->Resize(width, height);
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