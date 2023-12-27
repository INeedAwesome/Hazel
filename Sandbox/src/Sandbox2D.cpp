#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_Texture = Hazel::Texture2D::Create("assets/textures/checkerboard-pattern615x615.jpg");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
	Hazel::Renderer2D::DrawQuad({ -1.0f, 0 }, { 1, 1 }, { 1, 0, 0, 1 });
	Hazel::Renderer2D::DrawQuad({ 2.0f, 3.0f }, { 40, 2 }, { 0, 1, 0, 1 });
	Hazel::Renderer2D::DrawQuad({ -10.0f, 5.0f, -0.1f }, { 10, 10 }, m_Texture, { 0.5f, 0.5f, 1.0f, 1 });
	
	Hazel::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Sandbox2D");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
