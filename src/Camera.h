#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//defines options for camera movement
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};


//predefined options for the camera
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 7.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


//abstract camera class
class Camera
{
public:
	bool    m_isOrtho = true;
	int     m_windowWidth = 800.0f;
	int     m_windowHeight = 600.0f;
	virtual void processInput(GLFWwindow* window, float deltaTime) = 0;
	virtual void setOrthographic(float left, float right, float bottom, float top, float near, float far) = 0;
	virtual void setPosition(const glm::vec3& position) = 0;
	virtual void setPerspective () = 0;
	virtual glm::vec3 getLookAt() const = 0;
	virtual void enableMouse(GLFWwindow* window) = 0;
	virtual void disableMouse(GLFWwindow* window) = 0;
	virtual void printPosition() = 0;
	virtual glm::vec3 getPosition() const = 0;
	virtual glm::mat4 getViewMatrix() = 0;
	virtual glm::mat4 getProjectionMatrix() = 0;
	virtual ~Camera() = default;
};


//given static camera class which CANNOT move
class staticCamera : public Camera
{
private:
	glm::vec3 m_position;
	glm::vec3 m_target;
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0);

	glm::mat4 m_projection;
	glm::mat4 m_view;

	float m_fov = 45.0f;
	float m_aspectRatio = 800.0f / 600.0f;
	float m_near = 0.1f;
	float m_far = 100.0f;

public:

	//constructor
	staticCamera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f), const glm::vec3& target = glm::vec3(0.0f, 0.0f, 0.0f));

	//basic ovverride functions
	void processInput(GLFWwindow* window, float deltaTime) override;
	glm::mat4 getViewMatrix() override;
	glm::mat4 getProjectionMatrix() override;
	void enableMouse(GLFWwindow* window)  override;
	void disableMouse(GLFWwindow* window) override;

	//functions that change the view of the camera
	void setOrthographic(float left, float right, float bottom, float top, float near, float far) override;
	void setPerspective() override;
	glm::vec3 getLookAt() const override;
	void setPosition(const glm::vec3& position) override;
	glm::vec3 getPosition() const override;
	void printPosition() override;
};

class flyCamera : public Camera
{
private:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
	glm::vec3 m_cameraPos;     //camera position in world space in reference to world origin
	glm::vec3 m_cameraFront;   //defines the vector that is pointing in front of the camera
	glm::vec3 m_cameraUp;      //a vector to specify the position that is above the camera
	glm::vec3 m_WorldUp;       //a vector to specify the position that is above the camera
	glm::vec3 m_cameraRight;

	float m_yaw;
	float m_pitch;

	float m_movementSpeed;
	float m_mouseSensitivity;
	float m_zoom;

	float m_lastX = 800 / 2.0f;
	float m_lastY = 600 / 2.0f;
	bool  m_firstMouse = true;

public:
	//constructor
	flyCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	//basic ovverride functions
	void processInput(GLFWwindow* window, float deltaTime) override;
	glm::mat4 getViewMatrix() override;
	glm::mat4 getProjectionMatrix() override;

	//view functions if needed
	void setOrthographic(float left, float right, float bottom, float top, float near, float far) override;
	void setPerspective() override;
	void setPosition(const glm::vec3& position) override;

	//functions to handle flying around
	void updateCameraVectors();
	glm::vec3 getLookAt() const override;
	void enableMouse(GLFWwindow* window)  override;
	void disableMouse(GLFWwindow* window) override;
	void printPosition() override;
	glm::vec3 getPosition() const override;


	//TODO: find a way to make zooming in possible
};
