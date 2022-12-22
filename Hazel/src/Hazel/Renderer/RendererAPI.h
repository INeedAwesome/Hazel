#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Hazel {

	class RendererAPI 
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1, DirectX11 = 2
		};

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void Resize(int width, int height) = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
		inline static std::string GetAPIName() 
		{ 
			if (s_API == API::None) { return "None"; }
			else if (s_API == API::OpenGL) { return "OpenGL";}
			else if (s_API == API::DirectX11) { return "DirectX 11";}
		}
	private:
		static API s_API;
	};
}

