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
}

void Scene_Menu::update()
{
    sRender();
    
}
void Scene_Menu::sRender()
{
    m_engine.renderer()->clear();
    m_engine.renderer()->updateCameraView();

    //set up mathimatical positions for a circle

    for (int i = 0; i <= 12; i++)
    {
        float angle = 2.0f * glm::pi<float>() * i / 12;
        float x = cos(angle);
        float z = sin(angle);

        m_engine.renderer()->drawCube(glm::vec3(x, 0, -5.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }


   //float angle = (float)glfwGetTime() * 1.5f;
   //m_engine.renderer()->drawCube(glm::vec3(0, 0, -5.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), angle);

}
void Scene_Menu::onEnd()
{

}

