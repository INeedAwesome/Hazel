#pragma once

#include <Hazel.h>

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override; // like on init
	virtual void OnDetach() override; // like on destroy/cleanup

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& e) override;

private:
	Hazel::OrthographicCameraController m_CameraController;
	Hazel::ShaderLibrary m_ShaderLibrary;

	// temporary
	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 1.0f };

};

