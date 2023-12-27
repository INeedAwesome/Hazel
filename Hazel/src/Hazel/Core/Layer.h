#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"

namespace Hazel {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {} // like on init
		virtual void OnDetach() {} // like on destroy/cleanup
		virtual void OnUpdate(Timestep ts) {} // every layer gets updated at the same time
		virtual void OnImGuiRender() {} // like OnUpdate but for imgui
		virtual void OnEvent(Event& e) {}
	
		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}


