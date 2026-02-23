
#include "Camera.h"
#include <iostream>

//-----------------------------Static Camera Functions------------------------------


staticCamera::staticCamera(const glm::vec3& position, const glm::vec3& target)
	:m_position(position), m_target(target)
{
	//set the intial view and set the perspective
	m_view = glm::lookAt(m_position, m_target, m_up);
	m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);
}

void staticCamera::processInput(GLFWwindow* window, float deltaTime)
{
	//the camera should not accept input as well
}

glm::mat4 staticCamera::getViewMatrix()
{
	if (m_isOrtho)
		return glm::mat4(1.0f); // identity - no camera transform for UI

	return glm::lookAt(m_position, m_target, m_up);
}

glm::mat4 staticCamera::getProjectionMatrix()
{
	return m_projection;
}

void staticCamera::setOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	m_projection = glm::ortho(left, right, bottom, top, near, far);
	m_isOrtho = true;
}

void staticCamera::setPerspective()
{
	m_projection = glm::perspective(glm::radians(m_fov), 800.0f / 600.0f, 0.1f, 100.0f);
	m_isOrtho = false;
}

void staticCamera::setPosition(const glm::vec3& position)
{
	m_position = position;
	m_view = glm::lookAt(m_position, m_target, m_up);
}

void staticCamera::printPosition()
{
	std::cout << "X: " << m_position.x << " Y: " << m_position.y << " Z: " << m_position.z << std::endl;
}

void staticCamera::enableMouse(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void staticCamera::disableMouse(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

//-----------------------------Fly Camera Functions------------------------------

flyCamera::flyCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	:m_mouseSensitivity(SENSITIVITY),
	m_movementSpeed(SPEED),
	m_zoom(ZOOM),
	m_cameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
	m_cameraPos = position;
	m_WorldUp = up;
	m_yaw = yaw;
	m_pitch = pitch;
	setPerspective();
	updateCameraVectors();
}

void flyCamera::processInput(GLFWwindow* window, float deltaTime)
{

	//handle the camera movement stuff
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (m_firstMouse)
	{
		m_lastX = xpos;
		m_lastY = ypos;
		m_firstMouse = false;
	}

	float xOffset = (xpos - m_lastX) * m_mouseSensitivity;
	float yOffset = (m_lastY - ypos) * m_mouseSensitivity;

	m_lastX = xpos;
	m_lastY = ypos;

	m_yaw += xOffset;
	m_pitch += yOffset;
	m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
	updateCameraVectors();


	//The things that handle the keyboard movement
	float velocity = m_movementSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_cameraPos += velocity * m_cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_cameraPos -= velocity * m_cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * velocity;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * velocity;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_cameraPos += m_cameraUp * velocity;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_cameraPos -= m_cameraUp * velocity;
}

glm::mat4 flyCamera::getViewMatrix()
{
	if (m_isOrtho)
		return glm::mat4(1.0f); // identity - no camera transform for UI

	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

glm::mat4 flyCamera::getProjectionMatrix()
{
	return m_projectionMatrix;
}

void flyCamera::setOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	m_projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
	m_isOrtho = true;
}

void flyCamera::setPerspective()
{
	m_projectionMatrix = glm::perspective(glm::radians(m_zoom), 800.0f / 600.0f, 0.1f, 100.0f);
	m_isOrtho = false;
}

void flyCamera::setPosition(const glm::vec3& position)
{
	m_cameraPos = position;
}

void flyCamera::updateCameraVectors()
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

void flyCamera::enableMouse(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_firstMouse = true;
}
void flyCamera::disableMouse(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void flyCamera::printPosition()
{
	std::cout << "X: " << m_cameraPos.x << " Y: " << m_cameraPos.y << " Z: " << m_cameraPos.z << std::endl;
}
