#include "BlackHole3D.h"
#include "Scene_Menu.h"

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

	registerAction(GLFW_KEY_ENTER, "CHANGE_SCENE");
	registerAction(GLFW_KEY_C,   "CHANGE_CAMERA");


	//lighting set up
	m_pointLight.position = glm::vec3(0.0, 8.0f, 0.0f);
	m_pointLight.color = glm::vec3(1.0, 1.0f, 1.0f);
	m_pointLight.ambient = 0.3f;
	m_pointLight.diffuse = 0.8f;
	m_pointLight.specular = 0.5f;

	//set up intial blackhole
	BlackHole3D bh =
	{
		glm::vec3(0.0f, 5.0f, 0.0f),
		5.0f
	};
	m_blackHoles.push_back(bh);
}

void BlackHole3D_Scene::update()
{
	sRender();
	sGUI();
}

void BlackHole3D_Scene::sRender()
{
	m_engine.renderer()->clear();
	m_camera->processInput(m_engine.getWindow(), m_engine.getDeltaTime());

	m_camera->setPerspective();
	m_engine.renderer()->updateMatrix(m_camera->getProjectionMatrix(), m_camera->getViewMatrix(), m_camera->getPosition());
	m_engine.renderer()->setLight(m_pointLight);

	//std::cout << m_engine.getFPS() << std::endl;

	//set up data to pass into space time grid
	std::vector<glm::vec3> positions;
	std::vector<float>     magntiudes;
	for (const BlackHole3D& bh : m_blackHoles)
	{
		positions.push_back(bh.position);
		magntiudes.push_back(bh.gridMag);
		//draw blackhole
		m_engine.renderer()->drawSphere(bh.position, 1.75f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0, m_engine.assets()->getTexture("VenusDirt"));

	}
	m_engine.renderer()->drawCurvedGrid(30, 0.5f, positions, magntiudes, glm::vec3(0.5f, 0.5f, 0.5f));
	m_engine.renderer()->disableLight();

}

void BlackHole3D_Scene::sUserInput(const Action& action)
{
	if (action.type() == "PRESSED")
	{
		if (action.name() == "CHANGE_SCENE")
		{
			m_engine.changeScene("MENU", std::make_shared<Scene_Menu>(m_engine), false); onEnd();
		}
		if (action.name() == "CHANGE_CAMERA")
		{
			//could use some work with the positioning stuff
			std::cout << "pressed tab" << std::endl;

			if (m_isCameraStatic)
			{
				std::cout << "changed camera fly" << std::endl;

				glm::vec3 cameraPos = m_camera->getPosition();
				//create the fly camera and then disable mouse
				m_camera = std::make_unique<flyCamera>(cameraPos);
				m_camera->disableMouse(m_engine.getWindow());
				m_isCameraStatic = false;
			}
			else
			{
				std::cout << "static camera fly" << std::endl;

				glm::vec3 cameraPos = m_camera->getPosition();
				glm::vec3 cameraLook = m_camera->getLookAt();

				//create the static camera, and enable mouse
				m_camera->enableMouse(m_engine.getWindow());
				m_camera = std::make_unique<staticCamera>(cameraPos, cameraLook);
				m_isCameraStatic = true;
			}
		}
	}
}

void BlackHole3D_Scene::sGUI()
{

	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
	ImGui::Begin("Simulation Control", &m_openGUI);

	if (ImGui::BeginTabBar("FunctionsTabs"))
	{

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
					BlackHole3D& bh = m_blackHoles[i];

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
						//ImGui::Text("Stats: ");
						//ImGui::Text("Schwarzschild radius: (%.1f)", bh.r_s);
						//ImGui::Text("Rendering radius    : (%.1f)", bh.getRenderRadius());
						//ImGui::Text("Total Mass Value    : (%.1f)", bh.mass);
						//ImGui::Text("Name                : (%s)", bh.name.c_str());

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
						if (ImGui::SliderFloat("##slider_x", &bh.position.x, -3, 3, "px: %.1f")) {}

						// Y input and slider
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth);
						if (ImGui::InputFloat("##input_y", &bh.position.y, 0.0f, 0.0f, "%.1f")) {}

						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth * 1.5f);
						if (ImGui::SliderFloat("##slider_y", &bh.position.y, -3, 3, "px: %.1f")) {}

						// z input and slider
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth);
						if (ImGui::InputFloat("##input_z", &bh.position.z, 0.0f, 0.0f, "%.1f")) {}

						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth * 1.5f);
						if (ImGui::SliderFloat("##slider_z", &bh.position.z, -3, 3, "px: %.1f")) {}

						//-----------------------------Resizing-------------------------------------
						ImGui::Separator();
						ImGui::Text("Scale: ");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200);
						float pixelRadius = 80;
						//if (ImGui::SliderFloat("##slider_scale", &pixelRadius, 1, 160, "px: %.1f")) { bh.calculateRenderScale(pixelRadius); }

						ImGui::TreePop();
					}
				}

				//if (ImGui::Button("Straight Rays")) { drawStraightRays(100); }
				//if (ImGui::Button("Circular Rays")) { drawCircularRays(glm::vec2(200.0f, 200.0f), 256); }

				ImGui::EndTabItem();
			}
		}
	}
	ImGui::EndTabBar();
	ImGui::End();
}

void BlackHole3D_Scene::onEnd()
{
}
