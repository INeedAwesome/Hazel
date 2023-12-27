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

	m_SquareVertexArray = Hazel::VertexArray::Create();
	float sqaureVertices[3 * 4] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f
	};
	Hazel::Ref<Hazel::VertexBuffer> squareVertexBuffer;
	squareVertexBuffer = Hazel::VertexBuffer::Create(sqaureVertices, sizeof(sqaureVertices));
	squareVertexBuffer->SetLayout({
		{ Hazel::ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

	unsigned int sqaureIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Hazel::Ref<Hazel::IndexBuffer> squareIndexBuffer;
	squareIndexBuffer = Hazel::IndexBuffer::Create(sqaureIndices, sizeof(sqaureIndices) / sizeof(unsigned int));
	m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

	m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");

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

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	auto flatColorShader = m_ShaderLibrary.Get("FlatColor");
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(flatColorShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(flatColorShader)->UploadUniform("u_Color", m_SquareColor);

	Hazel::Renderer::Submit(flatColorShader, m_SquareVertexArray);
	
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
