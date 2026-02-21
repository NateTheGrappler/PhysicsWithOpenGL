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
    m_engine.renderer()->drawRect(glm::vec2(400, 300), glm::vec2(100, 50), glm::vec3(0.5f, 0.0f, 0.5f));
}
void Scene_Menu::onEnd()
{

}

