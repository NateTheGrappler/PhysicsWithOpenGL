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
	registerAction(GLFW_KEY_C,     "CHANGE_CAMERA");
	registerAction(GLFW_KEY_P,     "PAUSE");


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

	//set up some cool background star objects
	m_bgStars.push_back({ glm::vec3(4.0f,   5.0f,  2.0f), 0.1f, glm::vec3(0.2f, 0.4f, 1.0f) }); // blue   - right side
	m_bgStars.push_back({ glm::vec3(-4.0f,  5.0f,  2.0f), 0.1f, glm::vec3(1.0f, 0.3f, 0.1f) }); // orange - left side
	m_bgStars.push_back({ glm::vec3(0.0f,   5.0f,  2.0f), 0.1f, glm::vec3(0.1f, 0.8f, 0.3f) }); // green  - above
	m_bgStars.push_back({ glm::vec3(0.0f,   5.0f,  2.0f), 0.1f, glm::vec3(0.9f, 0.9f, 0.2f) }); // yellow - below
	m_bgStars.push_back({ glm::vec3(3.0f,   5.0f,  2.0f), 0.1f, glm::vec3(0.8f, 0.1f, 0.8f) }); // purple - upper right
	m_bgStars.push_back({ glm::vec3(-3.0f,  5.0f,  2.0f), 0.1f, glm::vec3(0.1f, 0.9f, 0.9f) }); // cyan   - lower left


	//set up rays
	//initRays(10, 0.5f);
	initStars(2000);
}

void BlackHole3D_Scene::update()
{
	for (lightRay3D& lr : m_lightRays)
	{
		if (lr.continueStep && !m_paused) { lr.step(m_engine.getDeltaTime(), m_blackHoles); }
	}

	sRender();
	sGUI();
}

void BlackHole3D_Scene::sRender()
{
	m_camera->processInput(m_engine.getWindow(), m_engine.getDeltaTime());
	m_camera->setPerspective();

	glm::vec3 bhPos = m_blackHoles[0].position;

	glm::mat4 captureProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(bhPos, bhPos + glm::vec3(1, 0, 0), glm::vec3(0,-1, 0)),
		glm::lookAt(bhPos, bhPos + glm::vec3(-1, 0, 0), glm::vec3(0,-1, 0)),
		glm::lookAt(bhPos, bhPos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
		glm::lookAt(bhPos, bhPos + glm::vec3(0,-1, 0), glm::vec3(0, 0,-1)),
		glm::lookAt(bhPos, bhPos + glm::vec3(0, 0, 1), glm::vec3(0,-1, 0)),
		glm::lookAt(bhPos, bhPos + glm::vec3(0, 0,-1), glm::vec3(0,-1, 0)),
	};

	std::vector<glm::vec3> positions;
	std::vector<float>     magnitudes;
	std::vector<float>     radii;
	for (const BlackHole3D& bh : m_blackHoles)
	{
		positions.push_back(bh.position);
		magnitudes.push_back(bh.gridMag);
		radii.push_back(bh.radius);
	}

	//------------------------------------------------------
	// PASS 1 - render scene into cubemap (no grid)
	//------------------------------------------------------
	for (int face = 0; face < 6; face++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_engine.renderer()->m_cubeMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
			m_engine.renderer()->m_cubeMapTexture, 0);

		glViewport(0, 0, 1024, 1024);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_engine.renderer()->updateMatrix(captureProj, captureViews[face], bhPos);
		m_engine.renderer()->setLight(m_pointLight);

		for (lightRay3D& lr : m_lightRays)
			m_engine.renderer()->drawTrail3D(lr.trail, lr.color);

		m_engine.renderer()->drawStars(m_starVAO, m_starCount, bhPos);

		for (const backGroundStar& bgStar : m_bgStars)
		{
			m_engine.renderer()->drawSphere(bgStar.position, bgStar.radius, bgStar.color);
			m_engine.renderer()->drawBackGroundStarGlow(bgStar.position, bgStar.radius * 4.0f, bgStar.color);

		}

		// accretion disk
		m_engine.renderer()->drawTorus(
			m_blackHoles[0].position,
			m_blackHoles[0].diskInner,
			m_blackHoles[0].diskOutter,
			(float)glfwGetTime() * 0.3f
		);

		m_engine.renderer()->disableLight();
	}

	//------------------------------------------------------
	// PASS 2 - ray trace fullscreen quad to screen
	//------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glm::ivec2 windowSize = m_engine.windowSize();
	glViewport(0, 0, windowSize.x, windowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_engine.renderer()->updateMatrix(
		m_camera->getProjectionMatrix(),
		m_camera->getViewMatrix(),
		m_camera->getPosition()
	);

	m_engine.renderer()->drawRayTracedBlackHole(
		bhPos,
		m_blackHoles[0].radius,
		m_camera->getPosition(),
		m_camera->getViewMatrix(),
		m_camera->getProjectionMatrix()
	);

	//------------------------------------------------------
	// PASS 3 - draw grid on top with normal camera matrices
	// depth test is re-enabled after ray trace so this works correctly
	//------------------------------------------------------
	m_engine.renderer()->setLight(m_pointLight);
	m_engine.renderer()->drawCurvedGrid(50, 0.5f, positions, magnitudes, radii, glm::vec3(0.5f, 0.5f, 0.5f));
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
		if (action.name() == "PAUSE")
		{
			m_paused = !m_paused;
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
						if (ImGui::SliderFloat("##slider_x", &bh.position.x, -10, 10, "px: %.1f")) {}

						// Y input and slider
						ImGui::Text("Y:");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth);
						if (ImGui::InputFloat("##input_y", &bh.position.y, 0.0f, 0.0f, "%.1f")) {}

						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth * 1.5f);
						if (ImGui::SliderFloat("##slider_y", &bh.position.y, -10, 10, "px: %.1f")) {}

						// z input and slider
						ImGui::Text("Z:");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth);
						if (ImGui::InputFloat("##input_z", &bh.position.z, 0.0f, 0.0f, "%.1f")) {}

						ImGui::SameLine();
						ImGui::SetNextItemWidth(controlWidth * 1.5f);
						if (ImGui::SliderFloat("##slider_z", &bh.position.z, -10, 10, "px: %.1f")) {}

						//-----------------------------Resizing-------------------------------------
						ImGui::Separator();
						ImGui::Text("Scale: ");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200);
						if (ImGui::SliderFloat("##slider_scale", &bh.radius, 0.5, 2.5, "px: %.1f")) { }


						ImGui::Separator();
						ImGui::Text("Magnitude: ");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200);
						if (ImGui::SliderFloat("##slider_mag", &bh.gridMag, 0.5, 20, "kge10: %.1f")) { }

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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glm::ivec2 windowSize = m_engine.windowSize();
	glViewport(0, 0, windowSize.x, windowSize.y);
}

void BlackHole3D_Scene::initRays(int gridSize, float spacing)
{
	//create a grid of the rays based on the specified values

	//halfsize so the grid starts at a middle origin
	float halfsize = gridSize * spacing / 2;

	for (int y = 0; y <= gridSize; y++)
	{
		float yPos = -halfsize + y * spacing;
		for (int z = 0; z <= gridSize; z++)
		{
			float zPos = -halfsize + z * spacing;

			lightRay3D lr =
			{
				glm::vec3(-10.0f, yPos + 5, zPos),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(0.7f, 0.7f, 0.0f)
			};
			m_lightRays.push_back(lr);
		}
	}
}
void BlackHole3D_Scene::initStars(int count)
{
	std::vector<float> starVertices;
	srand(42); // fixed seed so stars don't move each run

	for (int i = 0; i < count; i++)
	{
		// random point on a sphere surface using spherical coordinates
		float theta = ((float)rand() / RAND_MAX) * 2.0f * glm::pi<float>();
		float phi = acos(1.0f - 2.0f * ((float)rand() / RAND_MAX));
		float r = 30.0f + ((float)rand() / RAND_MAX) * 20.0f; // between 30 and 50 units out

		float x = r * sin(phi) * cos(theta);
		float y = r * sin(phi) * sin(theta);
		float z = r * cos(phi);

		// position
		starVertices.push_back(x);
		starVertices.push_back(y);
		starVertices.push_back(z);

		// random brightness for color (white to slightly yellow/blue)
		float brightness = 0.6f + ((float)rand() / RAND_MAX) * 0.4f;
		starVertices.push_back(brightness);
		starVertices.push_back(brightness);
		starVertices.push_back(brightness + ((float)rand() / RAND_MAX) * 0.1f);
	}

	m_starCount = count;

	glGenVertexArrays(1, &m_starVAO);
	glBindVertexArray(m_starVAO);

	glGenBuffers(1, &m_starVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_starVBO);
	glBufferData(GL_ARRAY_BUFFER, starVertices.size() * sizeof(float), starVertices.data(), GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

