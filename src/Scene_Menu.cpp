#include "Scene_Menu.h"

Scene_Menu::Scene_Menu(Engine& gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Menu::init()
{
    m_engine.renderer()->setBackgroundColor({0.1f, 0.1f, 0.1f, 1.0f});
    
    
    //TODO: register in function input
    registerAction(GLFW_KEY_R, "ROTATE_CLOCKWISE");
    registerAction(GLFW_KEY_T, "ROTATE_COUNTER_CLOCKWISE");

    //set the initial rotation positions and also apply a rotation of 0 to initialize all vectors
    calculateBasePositions();
}

void Scene_Menu::update()
{
    //run the function that takes in the input, and then the one that draws everythin
    sRender();    
}
void Scene_Menu::sRender()
{
    m_engine.renderer()->clear();
    m_engine.renderer()->updateCameraView();

    if (m_isRotating)
    {
        applyRotation();
        std::cout << "APPLYING ROTATION" << std::endl;
    }
    else { m_rotationProgress = 0.0f; }

    for (int i = 0; i < m_currentPositions.size(); i++)
    {
        float rotateAngle = glfwGetTime();

        //render the points with respect to the camera's origin at 0, 0, 0
        if (i % 2 == 0)
        {
            m_engine.renderer()->drawCube(m_currentPositions[i], glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
            //m_engine.renderer()->drawSphere(glm::vec3(x, 0, z - radius * 2), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
        }
        else
        {
            m_engine.renderer()->drawSphere(m_currentPositions[i], 0.5f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), rotateAngle);
        }
    }
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

    }
}
void Scene_Menu::onEnd()
{
}

void Scene_Menu::calculateBasePositions()
{
    m_currentPositions.clear();

    //do the math to set the vectors of position for the intital objects
    for (int i = 0; i <= m_numberOfObjects; i++)
    {
        float rotateAngle = glfwGetTime();;

        //set up points around the circle
        float angle = 2.0f * glm::pi<float>() * i / m_numberOfObjects;
        float x = cos(angle + rotateAngle / 3) * m_radius;
        float z = (sin(angle + rotateAngle / 3) * m_radius) - (m_radius * 2);

        //push back to a base position
        m_currentPositions.push_back(glm::vec3(x, 0, z));
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
        //m_currentPositions[i] = glm::mix(m_targetPositions[i], m_startPositions[i], m_rotationProgress);
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

