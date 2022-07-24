#include <Hazel.h>

#include "imgui/imgui.h"


class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override 
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB)) 
		{
			HZ_TRACE("Tab key is pressed");
		}

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Hazel::Event& e) override
	{
		if (e.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& event = (Hazel::KeyPressedEvent&)e;
			HZ_TRACE("{0}", (char)event.GetKeyCode());
			if (event.GetKeyCode() == HZ_KEY_ESCAPE)
			{
				auto& app = Hazel::Application::Get();
				app.OnEvent(Hazel::WindowCloseEvent);
			}
		}
	}

private:

};

class Sandbox : public Hazel::Application 
{
public:
	Sandbox()
	{ 
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
		
	}

};

Hazel::Application* Hazel::CreateApplication()
{ 
	return new Sandbox();
}