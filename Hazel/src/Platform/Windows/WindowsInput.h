#pragma once

#include "Hazel/Input.h"

namespace Hazel {

	class WindowsInput : public Input 
	{
	public:

	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int keycode) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

	private:

	};

}