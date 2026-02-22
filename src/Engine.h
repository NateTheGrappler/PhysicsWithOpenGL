#pragma once

#include "Renderer.h"

#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>

class Scene;

using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

class Engine
{
private:
	GLFWwindow*					 m_window;
	std::shared_ptr<Renderer>    m_renderer;
	int							 m_WIDTH = 800;
	int							 m_HEIGHT = 600;
	bool						 m_running = true;
	std::string					 m_currentSceneName;
	SceneMap					 m_SceneMap;
	std::unordered_map<int, int> m_prevKeyStates;

	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;

public:

	Engine();

	void init();
	void run();
	std::shared_ptr<Scene> currentScene();
	void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene);

	void sUserInput(GLFWwindow* window);

	GLFWwindow* getWindow();
	bool isRunning();
	float getDeltaTime();
	int height() const { return m_HEIGHT;};
	int width()  const { return m_WIDTH; };
	std::shared_ptr<Renderer> renderer() const { return m_renderer; }
};