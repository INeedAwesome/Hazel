#include "hzpch.h"
#include "RenderCommand.h"

#include "Hazel/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/DirectX11/DirectX11RendererAPI.h"

namespace Hazel {
	
#ifdef HZ_DIRECTX11
	RendererAPI* RenderCommand::s_RendererAPI = new DirectX11RendererAPI();
#endif // HZ_DIRECTX12
#ifdef HZ_OPENGL
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
#endif // HZ_OPENGL

}