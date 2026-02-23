#include "Scene_Menu.h"

Scene_Menu::Scene_Menu(Engine& gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Menu::init()
{
    //preset the renderer to set the scene view
    m_engine.renderer()->setBackgroundColor({0.1f, 0.1f, 0.1f, 1.0f});


    //set the initial rotation positions and also apply a rotation of 0 to initialize all vectors
    calculateBasePositions();


    //init the camera and then allow user to use mouse bc the camera is static
    m_camera = std::make_unique<staticCamera>(glm::vec3(m_currentPositions[3].x - 0.1, m_currentPositions[3].y + 1.0, m_currentPositions[3].z + 4.5),
        glm::vec3(m_currentPositions[3].x + 0.1f, m_currentPositions[3].y, m_currentPositions[3].z));
    m_camera->setPerspective();
    glfwSetInputMode(m_engine.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    
    //TODO: register in function input
    registerAction(GLFW_KEY_LEFT,  "ROTATE_CLOCKWISE");
    registerAction(GLFW_KEY_RIGHT, "ROTATE_COUNTER_CLOCKWISE");
    registerAction(GLFW_KEY_P,     "CHANGE_PERSPECTIVE");
    registerAction(GLFW_KEY_ENTER, "PRINT_POSITION");
    registerAction(GLFW_KEY_L,     "CHANGE_STATIC");

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


    //draw out the specific shape of each of the things you want to make
    float rotateAngle = glfwGetTime();

    m_camera->setPerspective();
    m_engine.renderer()->updateMatrix(m_camera->getProjectionMatrix(), m_camera->getViewMatrix());
    m_engine.renderer()->drawCircle(m_currentPositions[0],  0.75f,                 glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
    m_engine.renderer()->drawSphere(m_currentPositions[1],  0.75f,                 glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
    m_engine.renderer()->drawRect  (m_currentPositions[2],  glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
    m_engine.renderer()->drawSphere(m_currentPositions[3],  0.75f,                 glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
    m_engine.renderer()->drawRect  (m_currentPositions[4],  glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
    m_engine.renderer()->drawRect  (m_currentPositions[5],  glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
    m_engine.renderer()->drawCube  (m_currentPositions[6],  glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
    m_engine.renderer()->drawCube  (m_currentPositions[7],  glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
    m_engine.renderer()->drawCircle(m_currentPositions[8],  0.75f,                 glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
    m_engine.renderer()->drawRect  (m_currentPositions[9],  glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
    m_engine.renderer()->drawRect  (m_currentPositions[10], glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);

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


    m_camera->setOrthographic(-10.0f, 10.0f, -10.0f, 10.0f, 0, 5.0f);
    m_engine.renderer()->updateMatrix(m_camera->getProjectionMatrix(), m_camera->getViewMatrix());

    //draw the hud sort of an ordeal
    //m_engine.renderer()->drawCircle(glm::vec3(2.0f, 1.0f, 0.0f), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
    m_engine.renderer()->drawTriangle(glm::vec3(7.0f, 0.0f, 0.0f), glm::vec2(3.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

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
                m_camera = std::make_unique<staticCamera>(glm::vec3(0.0f, 0.0f, 0.0f));
                m_cameraIsStatic = true;
            }
        }
        if (action.name() == "PRINT_POSITION")
        {
            m_camera->printPosition();
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
        float rotateAngle = glfwGetTime();;

        //set up points around the circle
        float angle = 2.0f * glm::pi<float>() * i / m_numberOfObjects;
        float x = cos(angle) * m_radius;
        float z = (sin(angle) * m_radius) - (m_radius * 2);

        //push back to a base position
        m_currentPositions.push_back(glm::vec3(x - 1, 0, z));
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
    }

}

