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
    registerAction(GLFW_KEY_R, "ROTATE");

    //set the initial rotation positions and also apply a rotation of 0 to initialize all vectors
    calculateBasePositions();
    applyRotation();

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

    for (int i = 0; i < m_currentPositions.size(); i++)
    {
        float rotateAngle = glfwGetTime();;

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


   //m_engine.renderer()->drawCube(glm::vec3(0, 0, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), angle);

}

void Scene_Menu::sUserInput(const Action& action)
{
    if (action.type() == "PRESSED")
    {
        if (action.name() == "ROTATE")
        {
            std::cout << "Pressed R" << "\n";

            //increment the rotation offset, and then call to apply that rotation
            applyRotation();
        }

    }
}
void Scene_Menu::onEnd()
{
}

void Scene_Menu::calculateBasePositions()
{
    m_basePositions.clear();

    //do the math to set the vectors of position for the intital objects
    for (int i = 0; i <= m_numberOfObjects; i++)
    {
        float rotateAngle = glfwGetTime();;

        //set up points around the circle
        float angle = 2.0f * glm::pi<float>() * i / m_numberOfObjects;
        float x = cos(angle + rotateAngle / 3) * m_radius;
        float z = (sin(angle + rotateAngle / 3) * m_radius) - (m_radius * 2);

        //push back to a base position
        m_basePositions.push_back(glm::vec3(x, 0, z));
    }
}
void Scene_Menu::applyRotation()
{
    //Clear current positions
    m_currentPositions.clear();

    //New first element becomes old second element
    for (int i = 1; i < m_basePositions.size(); i++) {
        m_currentPositions.push_back(m_basePositions[i]);
    }
    //Add the first element at the end
    m_currentPositions.push_back(m_basePositions[0]);

    //Update base positions for next rotation
    m_basePositions = m_currentPositions;
}

