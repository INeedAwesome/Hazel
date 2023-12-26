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

	float m_BGColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // { 0.1f, 0.1f, 0.1f, 0.1f }

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_HLogoTexture;
	Hazel::Ref<Hazel::Shader> m_Shader, m_SquareShader, m_TextureShader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray, m_SquareVertexArray;
	
	Hazel::OrthographicCamera m_OrthoCamera;
	Hazel::PerspectiveCamera m_PerspectiveCamera;
	glm::vec3 m_CameraPositionOrtho;
	glm::vec3 m_CameraPositionPerspective;
	float m_CameraSpeed = 5.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 45.0f;

	float m_SquareSpeed = 1.0f;
	glm::vec3 m_SquarePosition;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 1.0f };

};