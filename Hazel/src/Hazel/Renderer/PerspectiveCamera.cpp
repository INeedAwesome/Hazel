#include "hzpch.h"
#include "PerspectiveCamera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar)
		: m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar)), m_ViewMatrix(glm::mat4(1.0f))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(transform);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}