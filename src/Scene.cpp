#include "Scene.h"

Scene::Scene(Engine& physicsEngine)
	:m_engine(physicsEngine)
{
}

void Scene::setPaused(bool trueOrFalse)
{
	m_paused = trueOrFalse;
}

//TODO: implement action register functions
