#include "hzpch.h"
#include "RenderCommand.h"

#include "Hazel/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/DirectX12/DirectX12RendererAPI.h"

namespace Hazel {
	
#ifdef HZ_DIRECTX12
	RendererAPI* RenderCommand::s_RendererAPI = new DirectX12RendererAPI();
#endif // HZ_DIRECTX12
#ifdef HZ_OPENGL
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
#endif // HZ_OPENGL

}