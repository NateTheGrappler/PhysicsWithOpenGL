#include "Scene.h"

Scene::Scene(Engine& physicsEngine)
	:m_engine(physicsEngine)
{
}

void Scene::setPaused(bool trueOrFalse)
{
	m_paused = trueOrFalse;
}


void Scene::registerAction(int inputKey, const std::string& actionName)
{
	m_actionMap[inputKey] = actionName;
}
void Scene::registerMouseAction(int mouseKey, const std::string& actionName)
{
	m_mouseActionMap[mouseKey] = actionName;
}

//TODO: implement action register functions
