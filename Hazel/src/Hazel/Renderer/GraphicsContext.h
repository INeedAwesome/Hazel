#pragma once

#include "Hazel/Core/Window.h"

namespace Hazel {

	class GraphicsContext
	{
	public:
		virtual void Init(const WindowProps& props) = 0;
		virtual void SwapBuffers() = 0;
	};

}