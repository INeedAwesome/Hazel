#pragma once

#include "Hazel/Layer.h"

#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/ApplicationEvent.h"

#ifdef HZ_DIRECTX11
	#include "Platform/DirectX11/DirectX11HeaderInstance.h"
#endif // HZ_DIRECTX11


namespace Hazel {

	class ImGuiLayer : public Hazel::Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
#ifdef HZ_DIRECTX11
		DirectX11HeaderInstance* m_D3DInstance = &DirectX11HeaderInstance::GetInstance();
#endif // HZ_DIRECTX11

	};
}

