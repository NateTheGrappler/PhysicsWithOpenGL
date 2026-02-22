#pragma once

#include "Scene.h"
#include <map>
#include <memory>

class Scene_Menu : public Scene
{
private:
    std::vector<glm::vec3> m_basePositions;
    std::vector<glm::vec3> m_currentPositions;
    unsigned int           m_numberOfObjects = 9;
    float                  m_radius = 5.0f;
    unsigned int           m_rotationOffset = 0;

public:
    Scene_Menu(Engine& gameEngine);

    void init();
    void update() override;
    void sRender() override;
    void sUserInput(const Action& action) override;
    void onEnd() override;

    //position calculation things
    void calculateBasePositions();
    void applyRotation();
};