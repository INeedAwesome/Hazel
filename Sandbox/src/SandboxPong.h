#pragma once

#include <Hazel.h>

class SandboxPong : public Hazel::Layer
{
public:
	SandboxPong();
	virtual ~SandboxPong() = default;

	virtual void OnAttach() override; // like on init
	virtual void OnDetach() override; // like on destroy/cleanup

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& e) override;

	void ResetGame();
	void MoveBall(Hazel::Timestep ts);
	void CheckCollisionForBall(Hazel::Timestep ts);

private:
	Hazel::OrthographicCameraController m_CameraController;

	bool m_Playing = false;

	uint32_t m_PlayerLeftPoints = 0;
	uint32_t m_PlayerRightPoints = 0;

	float m_BallSpeed = 1.5f;

	bool m_BallLeft = true;
	bool m_BallUp = true;

	glm::vec2 m_BallPosition = {0, 0};
	glm::vec2 m_BallSize = { 0.05f, 0.05f };

	glm::vec2 m_LeftPaddlePosition = { -1.5f, 0};
	glm::vec2 m_RightPaddlePosition = { 1.5f, 0};

	glm::vec2 m_PaddleSize = { 0.05f, 0.4f };

};

