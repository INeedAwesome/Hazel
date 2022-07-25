#include "SandboxApp.h"

#include "imgui/imgui.h"

#include "GLFW/include/GLFW/glfw3.h"


ExampleLayer::ExampleLayer()
	: Layer("SandboxApp")
{
}

void ExampleLayer::OnUpdate()
{
	
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Sandbox App!");
	ImGui::ColorEdit4("Color", colors);
	ImGui::End();
}

void ExampleLayer::OnEvent(Hazel::Event& e)
{
	if (e.GetEventType() == Hazel::EventType::KeyPressed) // checks for keyPressed events
	{
		Hazel::KeyPressedEvent& event = (Hazel::KeyPressedEvent&)e; // convert to keyPressedEvent
		if (event.GetKeyCode() == HZ_KEY_ESCAPE) // if key is escape
		{
			Hazel::Application& app = Hazel::Application::Get(); // get application
			app.StopRunning(); // stop app
		}
	}
}


Hazel::Application* Hazel::CreateApplication()
{ 
	return new Sandbox();
}

Sandbox::Sandbox()
{
	PushLayer(new ExampleLayer());
}

Sandbox::~Sandbox()
{

}