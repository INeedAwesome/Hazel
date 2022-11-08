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

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& e) override;

	bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event);
private:

	float m_BGColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // { 0.1f, 0.1f, 0.1f, 0.1f }
	
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;

	std::shared_ptr<Hazel::Shader> m_SquareShader;
	std::shared_ptr<Hazel::VertexArray> m_SquareVertexArray;

	
	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 45.0f;

};