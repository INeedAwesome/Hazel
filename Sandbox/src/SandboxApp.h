#pragma once

#include <Hazel.h>

class Sandbox : public Hazel::Application
{
public:
	Sandbox();
	~Sandbox();

private:

};

class ExampleLayer : public Hazel::Layer 
{
public:
	ExampleLayer();
	//~ExampleLayer();

	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& e) override;
private:

	float colors[4] = { 0.1f, 0.1f, 0.1f, 1 };
};