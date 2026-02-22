#include "Engine.h"
#include "Scene_Menu.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;


Engine::Engine()
{
	init();
}

void Engine::init()
{
	//create all of the necessary things to render a window
	glfwInit();                                                                                          
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                                                       
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                                                       
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(m_WIDTH, m_HEIGHT, "Physics Simulations", NULL, NULL);

	//error handling for window also setting some basic stuff for the window
	if (m_window == NULL)
	{
		std::cout << "Failed to create the window you goober" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	//outline the input things for glfw
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	//load GLAD and set viewport
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))                                         
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return ;
	}
	glViewport(0, 0, m_WIDTH, m_HEIGHT);

	//creater renderer
	m_renderer = std::make_shared<Renderer>();

	//change the scene pointer to be the new menu scene
	changeScene("MENU", std::make_shared<Scene_Menu>(*this), false);
}

void Engine::run()
{
	while (isRunning() && !glfwWindowShouldClose(m_window))
	{
		//calculate delta time
		float currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		sUserInput(m_window);

		//run the update scene for the scene that you currently ahve
		currentScene()->update();

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	//close glfw cleanly
	glfwTerminate();
}

std::shared_ptr<Scene> Engine::currentScene()
{
	return m_SceneMap[m_currentSceneName];
}

void Engine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	if (endCurrentScene)
	{
		m_SceneMap.erase(m_currentSceneName);
	}
	m_SceneMap[sceneName] = scene;
	m_currentSceneName = sceneName;
}

void Engine::sUserInput(GLFWwindow* window)
{
	//if escape key pressed, close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		//get the polygon mode
		GLint polygonMode[2];
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode[0]);

		if (polygonMode[0] == GL_LINE)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (polygonMode[0] == GL_FILL)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_renderer->getCamera().ProcessKeyboard(FORWARD, m_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_renderer->getCamera().ProcessKeyboard(BACKWARD, m_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_renderer->getCamera().ProcessKeyboard(LEFT, m_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_renderer->getCamera().ProcessKeyboard(RIGHT, m_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_renderer->getCamera().ProcessKeyboard(UP, m_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		m_renderer->getCamera().ProcessKeyboard(DOWN, m_deltaTime);
	}
}

GLFWwindow* Engine::getWindow()
{
	return m_window;
}
bool Engine::isRunning()
{
	return m_running;
}
float Engine::getDeltaTime()
{
	return m_deltaTime;
}



//functions defined here that run primarily within opengl
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//the functions are called whenever the mouse is moved in the glfw window
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY; //reversed because y-coordinate go from bottom to top

	lastX = xpos;
	lastY = ypos;

	engine->renderer()->getCamera().ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

	engine->renderer()->getCamera().ProcessMouseScroll(static_cast<float>(yoffset));
}
