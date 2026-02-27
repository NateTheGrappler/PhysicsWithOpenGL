//GENERAL IDEAS FOR DIFFERENT PHYSICS SIMULATIONS
//2d Simulation of blackholes
//3d Simulation of blackholes
//2d Simulation of gravity/solar system
//3d Simulation of gravity/solar system
//2d simulation of some sort of a cloth/rope physics
//2d simulation of sand/grains, maybe make it 3d or something
//3d simulation of balls inside of a box (like a cube with a ball in it that you can shake)
//2d simulation of a pendulum or something of the sort
//2d simulation of a spring sort of an ordeal
//maybe do some fun shit with waves
//try out some fluid simulation maybe


#include "Scene_Menu.h"
#include <string>
#include <random>
#include "BlackHole2D.h"

Scene_Menu::Scene_Menu(Engine& gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Menu::init()
{
    //set the initial rotation positions and also apply a rotation of 0 to initialize all vectors
    m_engine.renderer()->setBackgroundColor({0.1f, 0.1f, 0.1f, 1.0f});
    calculateBasePositions();

    //init the camera and then allow user to use mouse bc the camera is static
    m_camera = std::make_unique<staticCamera>(glm::vec3(m_currentPositions[3].x - 0.5, m_currentPositions[3].y + 1, m_currentPositions[3].z + 4.5),
        glm::vec3(m_currentPositions[3].x, m_currentPositions[3].y, m_currentPositions[3].z));
    glfwSetInputMode(m_engine.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    //load in needed assets
    m_engine.assets()->loadTexture("VenusDirt", "src/res/texture/VenusDirt.png");
    m_engine.assets()->loadTexture("topBanner", "src/res/texture/topBanner.png");
    m_engine.assets()->loadTexture("RightArrow", "src/res/texture/RightArrow.png");
    m_engine.assets()->loadTexture("BlackHoleSide", "src/res/texture/BlackHoleSide.png");
    
    registerAction(GLFW_KEY_LEFT,  "ROTATE_CLOCKWISE");
    registerAction(GLFW_KEY_RIGHT, "ROTATE_COUNTER_CLOCKWISE");
    registerAction(GLFW_KEY_P,     "CHANGE_PERSPECTIVE");
    registerAction(GLFW_KEY_ENTER, "ENTER_SCENE");
    registerAction(GLFW_KEY_L,     "CHANGE_STATIC");


    //set up a pointlight struct for fragment shader
    m_pointLight.position = glm::vec3(0.0, 5.0f, 0.0f);
    m_pointLight.color    = glm::vec3(1.0, 1.0f, 1.0f);
    m_pointLight.ambient  = 0.1f;
    m_pointLight.diffuse  = 0.8f;
    m_pointLight.specular = 0.5f;

    initStars();

}

void Scene_Menu::update()
{
    //run the function that takes in the input, and then the one that draws everythin
    sRender();  

}
void Scene_Menu::sRender()
{
    m_engine.renderer()->clear();
    m_camera->processInput(m_engine.getWindow(), m_engine.getDeltaTime());

    if (m_isRotating)     { applyRotation(); }
    else                  { m_rotationProgress = 0.0f; }

    //draw the stars
    m_camera->setPerspective();
    m_engine.renderer()->updateMatrix(m_camera->getProjectionMatrix(), m_camera->getViewMatrix(), m_camera->getPosition());
    m_engine.renderer()->drawStars(m_starVAO, m_starCount, m_camera->getPosition());

    

    
    //draw every option in the circle
    for (int i = 0; i < m_currentPositions.size(); i++)
    {
        float rotateAngle = glfwGetTime();

        //draw the circles
        if (i == 0 || i == 8)
        {
            m_engine.renderer()->drawCircle(m_currentPositions[i], 0.75f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle, m_engine.assets()->getTexture("VenusDirt"));
            renderSelection(i, "circle", rotateAngle);
        }
        //the spheres
        if (i == 1 || i == 3)
        {
            m_engine.renderer()->setLight(m_pointLight);
            m_engine.renderer()->drawSphere(m_currentPositions[i], 0.75f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle, m_engine.assets()->getTexture("VenusDirt"));
            renderSelection(i, "sphere", rotateAngle);
            m_engine.renderer()->disableLight();
        }
        //the cubes
        if (i == 7 || i == 6)
        {
            m_engine.renderer()->setLight(m_pointLight);
            m_engine.renderer()->drawCube(m_currentPositions[i], glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle, m_engine.assets()->getTexture("VenusDirt"));
            m_engine.renderer()->disableLight();
            renderSelection(i, "cube", rotateAngle);

        }
        //the rectangles
        if (i == 2 || i == 4 || i == 5 || i == 9 || i == 10)
        {
            m_engine.renderer()->drawRect(m_currentPositions[i], glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle, m_engine.assets()->getTexture("VenusDirt"));
            renderSelection(i, "rect", rotateAngle);

        }
    }

    //2d Stuff / doing the actual hud for the user
    glDisable(GL_DEPTH_TEST);
    m_camera->setOrthographic(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    m_engine.renderer()->updateMatrix(m_camera->getProjectionMatrix(), m_camera->getViewMatrix(), m_camera->getPosition());
    m_engine.renderer()->disableLight();
    m_engine.renderer()->drawTriangle(glm::vec3(720.0f, 75.0f, 0.0f),  glm::vec2(80.0f, 80.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(-90.0f), m_engine.assets()->getTexture("RightArrow"));
    m_engine.renderer()->drawTriangle(glm::vec3(80.0f,  75.0f, 0.0f),  glm::vec2(80.0f, 80.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(90.0f), m_engine.assets()->getTexture("RightArrow"));
    m_engine.renderer()->drawRect    (glm::vec3(400.0f, 525.0f, 0.0f), glm::vec2(700.0f, 80.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0, m_engine.assets()->getTexture("topBanner"));
    m_engine.renderer()->drawText("Physics Simulations w/ Opengl", glm::vec2(145.0f, 510), 0.9f, glm::vec3(0.9f, 0.9f, 0.9f));
    drawSimulationName();
    glEnable(GL_DEPTH_TEST);
}


void Scene_Menu::sUserInput(const Action& action)
{
    if (action.type() == "PRESSED")
    {
        if ((action.name() == "ROTATE_CLOCKWISE" || action.name() == "ROTATE_COUNTER_CLOCKWISE") && !m_isRotating)
        {
            std::cout << "Pressed R" << "\n";

            //calculate the rotation process and start rotation
            calculateTargetPosition(action.name());
            m_isRotating = true;
        }
        if (action.name() == "CHANGE_PERSPECTIVE")
        {
            std::cout << "changed perspective" << std::endl;
            if(m_camera->m_isOrtho) { m_camera->setPerspective(); }
            else                    { m_camera->setOrthographic(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f); }
            
            
        }
        if (action.name() == "CHANGE_STATIC")
        {
            if(m_cameraIsStatic) 
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
                m_camera = std::make_unique<staticCamera>(glm::vec3(m_currentPositions[3].x - 0.1, m_currentPositions[3].y + 1.0, m_currentPositions[3].z + 4.5),
                    glm::vec3(m_currentPositions[3].x + 0.1f, m_currentPositions[3].y, m_currentPositions[3].z));
                m_cameraIsStatic = true;
            }
        }
        if (action.name() == "ENTER_SCENE")   { changeScene(); }

        if (action.name() == "MOUSE_LEFT_CLICKED")
        {
            std::cout << "CLICKED MOUSE AT X: " <<  action.pos().x << " Y: " << action.pos().y << "\n";
            checkButtonCollision(action.pos());
            if (detectHover(action.pos().x, action.pos().y)) { changeScene(); }
        }

    }
}
void Scene_Menu::onEnd()
{
}

//rotation logic
void Scene_Menu::calculateBasePositions()
{
    m_currentPositions.clear();

    //do the math to set the vectors of position for the intital objects
    for (int i = 0; i < m_numberOfObjects; i++)
    {
        //set up points around the circle
        float angle = 2.0f * glm::pi<float>() * i / m_numberOfObjects;
        float x = cos(angle) * m_radius;
        float z = (sin(angle) * m_radius) - (m_radius * 2);

        //push back to a base position
        m_currentPositions.push_back(glm::vec3(x + 1.5, 0, z));
    }
}
void Scene_Menu::applyRotation()
{
    if (!m_isRotating) return;

    //update rotation progress
    m_rotationProgress += m_rotationSpeed * m_engine.getDeltaTime();

    //check to see if rotation is complete
    if (m_rotationProgress >= 1.0f)
    {
        m_rotationProgress = 1.0f;
        m_isRotating = false;
    }

    for (int i = 0; i < m_currentPositions.size(); i++)
    {
        m_currentPositions[i] = glm::mix(m_startPositions[i], m_targetPositions[i], m_rotationProgress);
    }

}
void Scene_Menu::calculateTargetPosition(const std::string& direction)
{
    if (direction == "ROTATE_CLOCKWISE")
    {
        //Clear current positions
        m_targetPositions.clear();

        //New first element becomes old second element
        for (size_t i = 1; i < m_currentPositions.size(); i++)
        {
            m_targetPositions.push_back(m_currentPositions[i]);
        }
        //Add the first element at the end
        m_targetPositions.push_back(m_currentPositions[0]);

        //store current position for interporlation
        m_startPositions = m_currentPositions;


        m_currentLookIndex -= 1;
        if (m_currentLookIndex < 0) { m_currentLookIndex = 10; }
    }
    else if (direction == "ROTATE_COUNTER_CLOCKWISE")
    {
        m_targetPositions.clear();

        m_targetPositions.push_back(m_currentPositions[m_currentPositions.size() - 1]);
        for (size_t i = 0; i < m_currentPositions.size() - 1; i++)
        {
            m_targetPositions.push_back(m_currentPositions[i]);
        }

        m_startPositions = m_currentPositions;

        m_currentLookIndex += 1;
        if (m_currentLookIndex > 10) { m_currentLookIndex = 0; }
    }

}

//handles the button inputs
void Scene_Menu::checkButtonCollision(const glm::vec2& pos)
{

    //the right hand side button
    if (pos.x < 760 && pos.x > 680 && pos.y > 35 && pos.y < 115 && !m_isRotating)
    {
        calculateTargetPosition("ROTATE_CLOCKWISE");
        m_isRotating = true;
    }
    //handle rotation in the other direction
    if (pos.x < 120 && pos.x > 40 && pos.y > 35 && pos.y < 115 && !m_isRotating)
    {
        calculateTargetPosition("ROTATE_COUNTER_CLOCKWISE");
        m_isRotating = true;
    }
}
bool Scene_Menu::detectHover(double xpos, double ypos)
{

    if (xpos < 500 && xpos > 300 && ypos < 400 && ypos > 200)
    {
        return true;
    }
    return false;
}

void Scene_Menu::renderSelection(int currentIndex, std::string shape, float rotateAngle)
{
    //query glfw for the location of the mouse
    double mouseX, mouseY;
    glfwGetCursorPos(m_engine.getWindow(), &mouseX, &mouseY);

    if (currentIndex == m_currentLookIndex && detectHover(mouseX, mouseY) && shape == "rect")
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_engine.renderer()->drawRect(m_currentPositions[currentIndex], glm::vec2(1.5f, 1.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (currentIndex == m_currentLookIndex && detectHover(mouseX, mouseY) && shape == "sphere")
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_engine.renderer()->drawSphere(m_currentPositions[currentIndex], 1.05f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle * 1.5);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (currentIndex == m_currentLookIndex && detectHover(mouseX, mouseY) && shape == "cube")
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_engine.renderer()->drawCube(m_currentPositions[currentIndex], glm::vec2(1.5f, 1.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle * 1.5);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (currentIndex == m_currentLookIndex && detectHover(mouseX, mouseY) && shape == "circle")
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        m_engine.renderer()->drawSphere(m_currentPositions[currentIndex], 1.05f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }



}
void Scene_Menu::drawSimulationName()
{
    double mouseX, mouseY;
    glfwGetCursorPos(m_engine.getWindow(), &mouseX, &mouseY);

    bool hovered = detectHover(mouseX, mouseY);

    //do the draw calls based on what the hovered item is
    if (m_currentLookIndex == 0  && hovered) { m_engine.renderer()->drawText("2D Black Hole Simulation", glm::vec2(180.0f, 60), 1.0f, glm::vec3(0.9, 0.9, 0.9));}
    if (m_currentLookIndex == 1  && hovered) { m_engine.renderer()->drawText("3D Black Hole Simulation", glm::vec2(180.0f, 60), 1.0f, glm::vec3(0.9, 0.9, 0.9));}
    if (m_currentLookIndex == 2  && hovered) { m_engine.renderer()->drawText("2D Solar System Gravity" , glm::vec2(180.0f, 60), 1.0f, glm::vec3(0.9, 0.9, 0.9));}
    if (m_currentLookIndex == 3  && hovered) { m_engine.renderer()->drawText("3D Solar System Gravity" , glm::vec2(180.0f, 60), 1.0f, glm::vec3(0.9, 0.9, 0.9));}
    if (m_currentLookIndex == 4  && hovered) { m_engine.renderer()->drawText("Cloth / Rope Simulation" , glm::vec2(190.0f, 60), 1.0f, glm::vec3(0.9, 0.9, 0.9));}
    if (m_currentLookIndex == 5  && hovered) { m_engine.renderer()->drawText("2D Sand Simulation"      , glm::vec2(215.0f, 60), 1.0f, glm::vec3(0.9, 0.9, 0.9));}
    if (m_currentLookIndex == 6  && hovered) { m_engine.renderer()->drawText("3D Ball Inside a box"    , glm::vec2(215.0f, 60), 1.0f, glm::vec3(0.9, 0.9, 0.9));}
    if (m_currentLookIndex == 7  && hovered) { m_engine.renderer()->drawText("Pendulum and Motion"     , glm::vec2(215.0f, 60), 1.0f, glm::vec3(0.9, 0.9, 0.9));}
    if (m_currentLookIndex == 8  && hovered) { m_engine.renderer()->drawText("Springs and Motion"      , glm::vec2(215.0f, 60), 1.0f, glm::vec3(0.9, 0.9, 0.9));}
    if (m_currentLookIndex == 9  && hovered) { m_engine.renderer()->drawText("2D Light And Sound Waves", glm::vec2(160.0f, 60), 1.0f, glm::vec3(0.9, 0.9, 0.9));}
    if (m_currentLookIndex == 10 && hovered) { m_engine.renderer()->drawText("2D Fluid Simulations"    , glm::vec2(220.0f, 60), 1.0f, glm::vec3(0.9, 0.9, 0.9));}
}

void Scene_Menu::changeScene()
{
    if (m_currentLookIndex == 0)  { m_engine.changeScene("2D_BLACK_HOLE", std::make_shared<BlackHole2D_Scene>(m_engine), false); }
    if (m_currentLookIndex == 1)  { m_engine.changeScene("2D_BLACK_HOLE", std::make_shared<BlackHole2D_Scene>(m_engine), false); }
    if (m_currentLookIndex == 2)  { m_engine.changeScene("2D_BLACK_HOLE", std::make_shared<BlackHole2D_Scene>(m_engine), false); }
    if (m_currentLookIndex == 3)  { m_engine.changeScene("2D_BLACK_HOLE", std::make_shared<BlackHole2D_Scene>(m_engine), false); }
    if (m_currentLookIndex == 4)  { m_engine.changeScene("2D_BLACK_HOLE", std::make_shared<BlackHole2D_Scene>(m_engine), false); }
    if (m_currentLookIndex == 5)  { m_engine.changeScene("2D_BLACK_HOLE", std::make_shared<BlackHole2D_Scene>(m_engine), false); }
    if (m_currentLookIndex == 6)  { m_engine.changeScene("2D_BLACK_HOLE", std::make_shared<BlackHole2D_Scene>(m_engine), false); }
    if (m_currentLookIndex == 7)  { m_engine.changeScene("2D_BLACK_HOLE", std::make_shared<BlackHole2D_Scene>(m_engine), false); }
    if (m_currentLookIndex == 8)  { m_engine.changeScene("2D_BLACK_HOLE", std::make_shared<BlackHole2D_Scene>(m_engine), false); }
    if (m_currentLookIndex == 9)  { m_engine.changeScene("2D_BLACK_HOLE", std::make_shared<BlackHole2D_Scene>(m_engine), false); }
    if (m_currentLookIndex == 10) { m_engine.changeScene("2D_BLACK_HOLE", std::make_shared<BlackHole2D_Scene>(m_engine), false); }
}

void Scene_Menu::initStars()
{
    //init random vals for the stars
    std::vector<float> stars;
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    std::uniform_real_distribution<float> distPoint(0.5f, 5.0f);

    //generate the stars based on this random distance
    for (int i = 0; i < m_starCount; i++)
    {
        glm::vec3 point;
        float pointSize;
        do
        {
            point = glm::vec3(dist(rng), dist(rng), dist(rng));
            pointSize = distPoint(rng);

        } while (glm::length(point) > 1.0f);


        //set the radius for the sphere
        point = glm::normalize(point) * 90.0f;

        stars.push_back(point.x);
        stars.push_back(point.y);
        stars.push_back(point.z);
        stars.push_back(pointSize);

    }


    //init all of the data for opengl
    glGenVertexArrays(1, &m_starVAO);
    glBindVertexArray(m_starVAO);

    glGenBuffers(1, &m_starVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_starVBO);
    glBufferData(GL_ARRAY_BUFFER, stars.size() * sizeof(float), stars.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

