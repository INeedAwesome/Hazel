#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override 
	{
		//HZ_INFO("ExampleLayer::OnUpdate");
	}

	void OnEvent(Hazel::Event& e) override
	{
		//HZ_INFO("{0}", e);
	}

private:

};

class Sandbox : public Hazel::Application 
{
public:
	Sandbox()
	{ 
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());

	}

	~Sandbox()
	{
		
	}

};

Hazel::Application* Hazel::CreateApplication()
{ 
	return new Sandbox();
}