#include "BlackHole2D.h"
#include "Scene_Menu.h"
#include <random>

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
	m_engine.assets()->loadTexture("BlackHole2D", "src/res/texture/blackHole2D.png");
	m_engine.assets()->loadTexture("BlackHole2D-Photon", "src/res/texture/BlackHole2D-Photon.png");

	//set up the black hole
	blackHole2D bh =
	{
		8.54 * std::pow(10, 36),
		glm::vec3(400.0f, 450.0f, 0),
		glm::vec3(1.0f, 0.0f, 2.0f),
		80
	};
	blackHole2D bh1 =
	{
		4.8e42,
		glm::vec3(400.0f, 150.0f, 0),
		glm::vec3(1.0f, 0.0f, 2.0f),
		80
	};
	m_blackHoles.push_back(bh);
	m_blackHoles.push_back(bh1);

	//drawStraightRays(100);
	drawCircularRays(glm::vec2(200.0f, 200.0f), 128);

}
void BlackHole2D_Scene::update()
{
	//update the position of the light ray

	for (int i = 0; i < m_lightRays.size(); i++)
	{
		if(m_lightRays[i].continueStep) { m_lightRays[i].step(m_engine.getDeltaTime(), m_blackHoles); }
	}
	sGUI();
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
		m_engine.renderer()->drawCircle(blackHole.position, blackHole.getRenderRadius(), blackHole.color, glm::vec3(1.0f, 1.0f, 1.0f), 0, m_engine.assets()->getTexture(blackHole.texture));
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
void BlackHole2D_Scene::sGUI()
{
	ImGui::Begin("Simulation Control");

	//set up tab items
	if (ImGui::BeginTabBar("FunctionsTabs"))
	{
		//the light ray handling
		if (ImGui::BeginTabItem("Light Rays"))
		{
			ImGui::Text("| Light Rays: %d", (int)m_lightRays.size());
			ImGui::SameLine();
			ImGui::Text("FPS: %.1f", m_engine.getFPS());

			ImGui::Separator();

			if (ImGui::Button("Straight Rays")) { drawStraightRays(100); }
			if (ImGui::Button("Circular Rays")) { drawCircularRays(glm::vec2(200.0f, 200.0f), 256); }

			ImGui::EndTabItem();
		}

		//creating / deleting blackholes
		if (ImGui::BeginTabItem("Black Holes"))
		{
			ImGui::Text("| Black Holes: %d", (int)m_blackHoles.size());
			ImGui::SameLine();
			ImGui::Text("FPS: %.1f", m_engine.getFPS());
			ImGui::Separator();



			if (ImGui::CollapsingHeader("Black Hole List", ImGuiTreeNodeFlags_DefaultOpen))
			{

				//black hole data & manipulation
				for (int i = 0; i < m_blackHoles.size(); i++)
				{
					blackHole2D& bh = m_blackHoles[i];

					std::string label = bh.name + " - " + std::to_string(i);
					if (ImGui::TreeNode(label.c_str()))
					{
						//-----------------------------Delete Button-------------------------------------
						ImGui::SameLine();
						if (ImGui::SmallButton(("Delete##" + std::to_string(i)).c_str()))
						{
							 m_blackHoles.erase(m_blackHoles.begin() + i);
							 i--;
						}

						//-----------------------------Stats-------------------------------------

						ImGui::Separator();
						ImGui::Text("Stats: ");
						ImGui::Text("Schwarzschild radius: (%.1f)", bh.r_s);
						ImGui::Text("Rendering radius    : (%.1f)", bh.getRenderRadius());
						ImGui::Text("Total Mass Value    : (%.1f)", bh.mass);
						ImGui::Text("Name                : (%s)", bh.name.c_str());

						//------------------------Position Changing------------------------------
						ImGui::Separator();
						ImGui::Text("Current Position: (%.1f, %.1f)", bh.position.x, bh.position.y);

						// Set consistent width for all controls
						const float controlWidth = 120.0f;

						// X input and slider
						ImGui::Text("X:");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth);
						if (ImGui::InputFloat("##input_x", &bh.position.x, 0.0f, 0.0f, "%.1f")) {}

						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth * 1.5f); // Sliders can be wider
						if (ImGui::SliderFloat("##slider_x", &bh.position.x, 0, (float)m_engine.windowSize().x, "px: %.1f")) {}

						// Y input and slider
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth);
						if (ImGui::InputFloat("##input_y", &bh.position.y, 0.0f, 0.0f, "%.1f")) {}

						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth * 1.5f);
						if (ImGui::SliderFloat("##slider_y", &bh.position.y, 0, (float)m_engine.windowSize().y, "px: %.1f")) {}

						//-----------------------------Resizing-------------------------------------
						ImGui::Separator();
						ImGui::Text("Scale: ");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200);
						float pixelRadius = 80;
						if (ImGui::SliderFloat("##slider_scale", &pixelRadius, 1, 160, "px: %.1f")) { bh.calculateRenderScale(pixelRadius); }

						ImGui::TreePop();
					}
				}

				//black hole creation
				ImGui::Separator();
				if (ImGui::Button("Create New"))
				{
					//randoms seed generation (once)
					static std::random_device rd; 
					static std::mt19937 rng(rd());
					std::uniform_real_distribution<float> dist(0.0f, 4.0f);
					std::uniform_real_distribution<float> tex(0.0f, 3.0f);

					//index for data clamped to be within bounds
					int index = static_cast<int>(dist(rng));
					int indexText = static_cast<int>(tex(rng));
					index = std::min(index, (int)m_bhData.size() - 1);

					std::pair<std::string, double> data = m_bhData[index];
					blackHole2D newBH =
					{
						data.second,
						glm::vec3(400.0f, 300.0f, 0),
						glm::vec3(1.0f, 0.0f, 2.0f),
						80
					};
					newBH.name = data.first;
					newBH.texture = m_bhTextureData[indexText];
					m_blackHoles.push_back(newBH);
				}
			}

			//if (ImGui::Button("Straight Rays")) { drawStraightRays(100); }
			//if (ImGui::Button("Circular Rays")) { drawCircularRays(glm::vec2(200.0f, 200.0f), 256); }

			ImGui::EndTabItem();
		}
	}


	ImGui::EndTabBar();
	ImGui::End();
}
void BlackHole2D_Scene::onEnd()
{
	std::cout << "exited out of scene BLACK_HOLE_2D" << std::endl;
}

//light related functions
void BlackHole2D_Scene::drawStraightRays(int num)
{
	m_lightRays.clear();

	for (int i = 15; i < 600; i += 600/ num)
	{
		//set up a light ray
		lightRayCartesian lr =
		{
			glm::vec3(0.0f, i, 0),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f)
		};
		m_lightRays.push_back(lr);
	}
}
void BlackHole2D_Scene::drawCircularRays(glm::vec2 origin, unsigned int count)
{
	m_lightRays.clear();

	for (int i = 0; i < count; i++)
	{
		float angle = 2.0f * glm::pi<float>() * i / count;
		float x = cos(angle);
		float y = sin(angle);

		//set up a light ray
		lightRayCartesian lr =
		{
			glm::vec3(origin.x, origin.y, 0),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec2(x, y)
		};
		m_lightRays.push_back(lr);
	}
}


