#include "hzpch.h"


#ifdef HZ_OPENGL
	#define IMGUI_IMPL_OPENGL_LOADER_GLAD
	#include "backends/imgui_impl_opengl3.cpp"
	#include "backends/imgui_impl_glfw.cpp" 
#endif // HZ_OPENGL

#ifdef HZ_DIRECTX11
	#include "backends/imgui_impl_dx11.cpp"
	#include "backends/imgui_impl_win32.cpp"
#endif // HZ_DIRECTX11


