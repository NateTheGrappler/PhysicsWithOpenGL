#include "Engine.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


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

	if (m_window == NULL)
	{
		std::cout << "Failed to create the window you goober" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	//load GLAD and set viewport
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))                                             //Initialize the glad library
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return ;
	}
	glViewport(0, 0, m_HEIGHT, m_WIDTH);
}

void Engine::run()
{
	while (isRunning() && !glfwWindowShouldClose(m_window))
	{
		//calculate delta time
		float currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		glfwPollEvents();

		//run the update scene for the scene that you currently ahve
		//currentScene()->update();

		glfwSwapBuffers(m_window);
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
