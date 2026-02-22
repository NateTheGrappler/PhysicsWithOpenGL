
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

class Camera
{
public:
	glm::mat4 m_viewMatrix;
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

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	//~Camera();

	glm::mat4 getViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);
	void ProcessMouseScroll(float yoffset);

private:
	void updateCameraVectors();
};