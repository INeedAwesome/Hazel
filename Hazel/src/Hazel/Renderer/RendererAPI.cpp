#include "hzpch.h"
#include "RendererAPI.h"

namespace Hazel {
	
#ifdef HZ_DIRECTX11
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::DirectX11;
#endif // HZ_DIRECTX12
#ifdef HZ_OPENGL
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#endif // HZ_OPENGL

}