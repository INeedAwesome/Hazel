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
	srand(time(NULL));
	ResetGame();
}

void SandboxPong::OnDetach()
{

}

void SandboxPong::OnUpdate(Hazel::Timestep ts)
{
	// Update

	if (Hazel::Input::IsKeyPressed(HZ_KEY_SPACE))
		m_Playing = true;

	if (m_Playing)
	{
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
	}


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

void SandboxPong::ResetGame()
{
	m_BallSpeed = 1.5f;
	m_BallPosition = {0, 0};
	m_LeftPaddlePosition = { -1.5f, 0 };
	m_RightPaddlePosition = { 1.5f, 0 };

	m_BallLeft = rand() % 2;
	m_BallUp = rand() % 2;

}

void SandboxPong::MoveBall(Hazel::Timestep ts)
{
	if (m_BallLeft)
		m_BallPosition.x += m_BallSpeed * ts;
	else
		m_BallPosition.x -= m_BallSpeed * ts;

	if (m_BallUp)
		m_BallPosition.y += m_BallSpeed * ts;
	else
		m_BallPosition.y -= m_BallSpeed * ts;

}

void SandboxPong::CheckCollisionForBall(Hazel::Timestep ts)
{
	float horizontalBoundarieBall = (1280.0f / 720.0f) - (m_BallSize.x / 2); // this works of the aspect ratio of the window
	float verticalBoundarieBall = 1 - (m_BallSize.y / 2);


	// Check if the ball is past the left paddle
	if (m_BallPosition.x <= -horizontalBoundarieBall)
	{
		m_PlayerRightPoints++;
		HZ_INFO("{}\t:\t{}\n", m_PlayerLeftPoints, m_PlayerRightPoints);
		ResetGame();
	}

	// Check if the ball is past the right paddle
	if (m_BallPosition.x >= horizontalBoundarieBall)
	{
		m_PlayerLeftPoints++;
		HZ_INFO("{}\t:\t{}\n", m_PlayerLeftPoints, m_PlayerRightPoints);
		ResetGame();
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

	// collision x-axis?
	bool leftCollisionX = m_BallPosition.x + (m_BallSize.x / 2) >= m_LeftPaddlePosition.x - (m_PaddleSize.x / 2)
		&& m_LeftPaddlePosition.x + (m_PaddleSize.x / 2) >= m_BallPosition.x - (m_BallSize.x / 2);

	// collision y-axis?
	bool leftCollisionY = m_BallPosition.y + (m_BallSize.y / 2) >= m_LeftPaddlePosition.y - (m_PaddleSize.y / 2)
		&& m_LeftPaddlePosition.y + (m_PaddleSize.y / 2) >= m_BallPosition.y - (m_BallSize.y / 2);

	// Turn off collision if the ball crosses the right side of the left paddle
	if (m_BallPosition.x + (m_BallSize.x / 2) < m_LeftPaddlePosition.x + (m_PaddleSize.x / 2))
	{
		leftCollisionX = false;
		leftCollisionY = false;
	}

	// collision only if on both axes
	if (leftCollisionX && leftCollisionY)
	{
		m_BallLeft = !m_BallLeft;
		m_BallSpeed += 0.05f;
	}


	// Collision with the right paddle
	// collision x-axis?
	bool rightCollisionX = m_BallPosition.x + (m_BallSize.x / 2) >= m_RightPaddlePosition.x - (m_PaddleSize.x / 2)
		&& m_RightPaddlePosition.x + (m_PaddleSize.x / 2) >= m_BallPosition.x - (m_BallSize.x / 2);

	// collision y-axis?
	bool rightCollisionY = m_BallPosition.y + (m_BallSize.y / 2) >= m_RightPaddlePosition.y - (m_PaddleSize.y / 2)
		&& m_RightPaddlePosition.y + (m_PaddleSize.y / 2) >= m_BallPosition.y - (m_BallSize.y / 2);

	// Turn off collision if the ball crosses the left side of the right paddle
	if (m_BallPosition.x + (m_BallSize.x / 2) < m_RightPaddlePosition.x - (m_PaddleSize.x / 2))
	{
		rightCollisionX = false;
		rightCollisionY = false;
	}

	// collision only if on both axes
	if (rightCollisionX && rightCollisionY)
	{
		m_BallLeft = !m_BallLeft;
		m_BallSpeed += 0.05f;
	}





}
