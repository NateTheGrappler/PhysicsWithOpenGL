#include "BlackHole3D.h"

BlackHole3D_Scene::BlackHole3D_Scene(Engine& m_engine)
	:Scene(m_engine)
{
	init();
}

void BlackHole3D_Scene::init()
{
	m_engine.renderer()->setBackgroundColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	
	m_camera = std::make_unique<flyCamera>(glm::vec3(0.0f, 0.0f, 5.0f));
	m_camera->disableMouse(m_engine.getWindow());


	//lighting set up
	m_pointLight.position = glm::vec3(0.0, 5.0f, 0.0f);
	m_pointLight.color = glm::vec3(1.0, 1.0f, 1.0f);
	m_pointLight.ambient = 0.3f;
	m_pointLight.diffuse = 0.8f;
	m_pointLight.specular = 0.5f;
}

void BlackHole3D_Scene::update()
{
	sRender();
}

void BlackHole3D_Scene::sRender()
{
	m_engine.renderer()->clear();
	m_camera->processInput(m_engine.getWindow(), m_engine.getDeltaTime());

	m_camera->setPerspective();
	m_engine.renderer()->updateMatrix(m_camera->getProjectionMatrix(), m_camera->getViewMatrix(), m_camera->getPosition());

	m_engine.renderer()->setLight(m_pointLight);
	m_engine.renderer()->drawSphere(glm::vec3(0.0f, 0.0f, 0.0f), 0.75f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0, m_engine.assets()->getTexture("VenusDirt"));
	m_engine.renderer()->disableLight();
}

void BlackHole3D_Scene::sUserInput(const Action& action)
{
}

void BlackHole3D_Scene::sGUI()
{
}

void BlackHole3D_Scene::onEnd()
{
}
