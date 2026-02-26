#include "BlackHole2D.h"
#include "Scene_Menu.h"

BlackHole2D_Scene::BlackHole2D_Scene(Engine& gameEngine)
	: Scene(gameEngine)
{
	init();
}

void BlackHole2D_Scene::init()
{
	m_engine.renderer()->setBackgroundColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	//set up camera object
	m_camera = std::make_unique<staticCamera>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glfwSetInputMode(m_engine.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//set up user input
	registerAction(GLFW_KEY_C,     "CHANGE_CAMERA");
	registerAction(GLFW_KEY_ENTER, "CHANGE_SCENE");

	//set up the black hole
	blackHole2D bh =
	{
		8.54 * std::pow(10, 36),
		glm::vec3(500.0f, 300.0f, 0),
		glm::vec3(1.0f, 0.0f, 2.0f),
		80
	};
	m_blackHoles.push_back(bh);
	std::cout << "Mass: " << bh.mass << " Radius: " << bh.r_s << " Render Radius: " << bh.getRenderRadius() << std::endl;

	//set up a light ray
	lightRay lr =
	{
		glm::vec3(0.0f, 300.0f, 0),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f)
	};
	m_lightRays.push_back(lr);

}

void BlackHole2D_Scene::update()
{
	//update the position of the light ray
	for (int i = 0; i < m_lightRays.size(); i++)
	{
		m_lightRays[i].step(m_renderScale, m_engine.getDeltaTime());
		std::cout << "X: " << m_lightRays[i].position.x << " Y: " << m_lightRays[i].position.y << std::endl;
	}
	sRender();
}

void BlackHole2D_Scene::sRender()
{
	m_engine.renderer()->clear();
	m_camera->processInput(m_engine.getWindow(), m_engine.getDeltaTime());

	glDisable(GL_DEPTH_TEST);
	m_camera->setOrthographic(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
	m_engine.renderer()->updateMatrix(m_camera->getProjectionMatrix(), m_camera->getViewMatrix(), m_camera->getPosition());
	m_engine.renderer()->disableLight();

	for (const blackHole2D& blackHole : m_blackHoles)
	{
		m_engine.renderer()->drawCircle(blackHole.position, blackHole.getRenderRadius(), blackHole.color);
	}

	for (int i = 0; i < m_lightRays.size(); i++)
	{
		m_engine.renderer()->drawTrail(m_lightRays[i].trail, m_lightRays[i].color);
	}

	glEnable(GL_DEPTH_TEST);
}

void BlackHole2D_Scene::sUserInput(const Action& action)
{
	if (action.type() == "PRESSED")
	{
		if (action.name() == "CHANGE_CAMERA")
		{
			if (m_cameraIsStatic)
			{
				std::cout << "changed camera fly" << std::endl;

				//create the fly camera and then disable mouse
				m_camera = std::make_unique<flyCamera>(glm::vec3(0.0f, 0.0f, 0.0f));
				m_camera->disableMouse(m_engine.getWindow());
				m_cameraIsStatic = false;
			}
			else
			{
				std::cout << "changed camera static" << std::endl;

				//create the static camera, and enable mouse
				m_camera->enableMouse(m_engine.getWindow());
				m_camera = std::make_unique<staticCamera>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				m_cameraIsStatic = true;
			}
		}
		if (action.name() == "CHANGE_SCENE") { m_engine.changeScene("MENU", std::make_shared<Scene_Menu>(m_engine), false); onEnd(); }
	}
}

void BlackHole2D_Scene::onEnd()
{
	std::cout << "exited out of scene BLACK_HOLE_2D" << std::endl;
}
