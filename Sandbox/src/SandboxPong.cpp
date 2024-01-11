#include "SandboxPong.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

SandboxPong::SandboxPong()
	: Layer("SandboxPong"), m_CameraController(1280.0f / 720.0f)
{

}

void SandboxPong::OnAttach()
{
	
}

void SandboxPong::OnDetach()
{

}

void SandboxPong::OnUpdate(Hazel::Timestep ts)
{
	// Update

	MoveBall(ts);
	CheckCollisionForBall(ts);

	if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
		m_LeftPaddlePosition.y += 1 * ts;

	if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
		m_LeftPaddlePosition.y -= 1 * ts;

	if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
		m_RightPaddlePosition.y += 1 * ts;

	if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
		m_RightPaddlePosition.y -= 1 * ts;


	// Render
	Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
	Hazel::Renderer2D::DrawQuad(m_BallPosition, m_BallSize, { 1, 1, 1, 1 });
	
	Hazel::Renderer2D::DrawQuad(m_LeftPaddlePosition, m_PaddleSize, { 1, 1, 1, 1 });
	Hazel::Renderer2D::DrawQuad(m_RightPaddlePosition, m_PaddleSize, { 1, 1, 1, 1 });

	Hazel::Renderer::EndScene();
}

void SandboxPong::OnImGuiRender()
{

}

void SandboxPong::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}

void SandboxPong::MoveBall(Hazel::Timestep ts)
{
	if (m_BallLeft)
		m_BallPosition.x += 1 * ts;
	else
		m_BallPosition.x -= 1 * ts;

	if (m_BallUp)
		m_BallPosition.y += 1 * ts;
	else
		m_BallPosition.y -= 1 * ts;

}

void SandboxPong::CheckCollisionForBall(Hazel::Timestep ts)
{
	float horizontalBoundarieBall = (1280.0f / 720.0f) - (m_BallSize.x / 2); // this works of the aspect ratio of the window
	float verticalBoundarieBall = 1 - (m_BallSize.y / 2);

	
	if (m_BallPosition.x >= horizontalBoundarieBall || m_BallPosition.x <= -horizontalBoundarieBall)
	{
		m_BallLeft = !m_BallLeft;
	}
	if (m_BallPosition.x >= horizontalBoundarieBall)
	{
		m_BallPosition.x = horizontalBoundarieBall;
	}
	if (m_BallPosition.x <= -horizontalBoundarieBall)
	{
		m_BallPosition.x = -horizontalBoundarieBall;
	}


	if (m_BallPosition.y >= verticalBoundarieBall || m_BallPosition.y <= -verticalBoundarieBall)
	{
		m_BallUp = !m_BallUp;
	}

	if (m_BallPosition.y >= verticalBoundarieBall)
	{
		m_BallPosition.y = verticalBoundarieBall;
	}
	if (m_BallPosition.y <= -verticalBoundarieBall)
	{
		m_BallPosition.y = -verticalBoundarieBall;
	}

	// Collision with the left paddle
	if (m_BallPosition.y <= m_LeftPaddlePosition.y + (m_PaddleSize.y / 2) && m_BallPosition.y >= -m_LeftPaddlePosition.y - (m_PaddleSize.y / 2))
	{
		HZ_INFO("between boundaries");
		if (m_BallPosition.x <= m_LeftPaddlePosition.x + (m_PaddleSize.x / 2))
		{
			m_BallLeft = !m_BallLeft;
			HZ_INFO("Collision LEFT");
		}
	}
	
	
	// Collision with the right paddle

}
