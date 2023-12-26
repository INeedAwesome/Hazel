#include "hzpch.h"
#include "RenderCommand.h"

#include "Hazel/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel {
#ifdef HZ_OPENGL
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
#endif // HZ_OPENGL

}