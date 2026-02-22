
#include "Camera.h"

//set the three vectors needed to have the camera be positioned in world space, and all other values needed to calculate the direction


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	:m_mouseSensitivity(SENSITIVITY),
	m_movementSpeed(SPEED),
	m_zoom(ZOOM),
	m_cameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
	m_cameraPos = position;
	m_WorldUp = up;
	m_yaw = yaw;
	m_pitch = pitch;
	updateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
{
	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;

	m_yaw += xoffset;
	m_pitch -= yoffset;

	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
		if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	m_zoom -= (float)yoffset;
	if (m_zoom < 1.0f)
	{
		m_zoom = 1.0f;
	}
	if (m_zoom > 45.0f)
	{
		m_zoom = 45.0f;
	}
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = m_movementSpeed * deltaTime;
	if (direction == FORWARD)
	{
		m_cameraPos += m_cameraFront * velocity;
	}
	if (direction == BACKWARD)
	{
		m_cameraPos -= m_cameraFront * velocity;
	}
	if (direction == LEFT)
	{
		m_cameraPos -= m_cameraRight * velocity;
	}
	if (direction == RIGHT)
	{
		m_cameraPos += m_cameraRight * velocity;
	}
	if (direction == DOWN)
	{
		m_cameraPos -= m_cameraUp * velocity;
	}
	if (direction == UP)
	{
		m_cameraPos += m_cameraUp * velocity;
	}
}



void Camera::updateCameraVectors()
{
	glm::vec3 front;
	//calculate the new front direction based on the euler angles
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	//normalize the vector
	m_cameraFront = glm::normalize(front);

	//recalculate the up and the right vector based on the new camera front vector
	m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_WorldUp));
	m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
}
