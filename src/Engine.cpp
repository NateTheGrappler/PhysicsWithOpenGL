#include "Engine.h"
#include "Scene_Menu.h"
#include "Action.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
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
	glfwSetScrollCallback(m_window, scroll_callback);

	//load GLAD and set viewport
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))                                         
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return ;
	}
	glViewport(0, 0, m_WIDTH, m_HEIGHT);

	//creater renderer and assets manager
	m_renderer = std::make_shared<Renderer>();
	m_assets   = std::make_shared<AssetManager>();


	//set up stuff for ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

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

		//calculate FPS:
		m_fpsTimer += m_deltaTime;
		m_frameCount++;
		if (m_fpsTimer >= 1.0f)
		{
			m_currentFps = m_frameCount / m_fpsTimer;
			m_frameCount = 0;
			m_fpsTimer = 0.0f;
		}

		//intake user input from keyboard or mouse
		glfwPollEvents();
		sUserInput(m_window);

		//ImGui Startup
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//run the update scene for the scene that you currently ahve
		currentScene()->update();

		//End imGui rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

void Engine::sUserInput(GLFWwindow* window)
{
	//if escape key pressed, close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
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

	//scene related key presses
	const ActionMap& actionMap = currentScene()->getActionMap();
	for (ActionMap::const_iterator it = actionMap.begin(); it != actionMap.end(); ++it)
	{
		int key = it->first;
		const std::string& actionName = it->second;

		int currentState = glfwGetKey(window, key);
		int prevState    = m_prevKeyStates.count(key) ? m_prevKeyStates[key] : GLFW_RELEASE;

		if (currentState == GLFW_PRESS && prevState == GLFW_RELEASE)
		{
			currentScene()->sUserInput(Action(actionName, "PRESSED"));
		}
		else if (currentState == GLFW_RELEASE && prevState == GLFW_PRESS)
		{
			currentScene()->sUserInput(Action(actionName, "RELEASED"));
		}

		m_prevKeyStates[key] = currentState;
	}

	//mouse button click kind of stuff
	int mouseState = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT);
	int prevMouseState = m_prevKeyStates.count(GLFW_MOUSE_BUTTON_LEFT) ? m_prevKeyStates[GLFW_MOUSE_BUTTON_LEFT] : GLFW_RELEASE;

	if (mouseState == GLFW_PRESS && prevMouseState == GLFW_RELEASE)
	{
		double mouseX, mouseY;
		glfwGetCursorPos(m_window, &mouseX, &mouseY);
		mouseY = 600 - mouseY;

		Action action("MOUSE_LEFT_CLICKED", "PRESSED", glm::vec2(mouseX, mouseY));
		currentScene()->sUserInput(action);
	}
	m_prevKeyStates[GLFW_MOUSE_BUTTON_LEFT] = mouseState;
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


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

	//engine->renderer()->getCamera().ProcessMouseScroll(static_cast<float>(yoffset));
}
