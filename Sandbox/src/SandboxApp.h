#pragma once

#include <Hazel.h>

#include <glm/gtc/matrix_transform.hpp>

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


	Hazel::OrthographicCameraController m_CameraController;

	Hazel::ShaderLibrary m_ShaderLibrary;
	Hazel::Ref<Hazel::Texture2D> m_Texture, m_HLogoTexture;
	Hazel::Ref<Hazel::Shader> m_TriangleShader, m_SquareShader;
	Hazel::Ref<Hazel::VertexArray> m_TriangleVertexArray, m_SquareVertexArray;

	glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 1.0f };

};