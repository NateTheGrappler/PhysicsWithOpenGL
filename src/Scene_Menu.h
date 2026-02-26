#pragma once

#include "Scene.h"
#include <map>
#include <memory>

class Scene_Menu : public Scene
{
private:
    std::vector<glm::vec3>   m_targetPositions;
    std::vector<glm::vec3>   m_currentPositions;
    std::vector<glm::vec3>   m_startPositions;
    unsigned int             m_numberOfObjects = 11;
    int                      m_currentLookIndex = 3;
    float                    m_radius = 5.0f;
    float                    m_rotationProgress = 0.0f;
    float                    m_rotationSpeed = 3.0f;

    std::unique_ptr<Camera>  m_camera;
    PointLight               m_pointLight;

    bool                     m_isRotating = false;
    bool                     m_cameraIsStatic = true;

    unsigned int m_starVAO, m_starVBO;
    unsigned int m_starCount = 3000;



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
    void calculateTargetPosition(const std::string& direction);
    void checkButtonCollision(const glm::vec2& pos);

    bool detectHover(double xpos, double ypos);
    void renderSelection(int currentIndex, std::string shape, float rotateAngle);
    void drawSimulationName();
    void changeScene();

    //stuff for the stars
    void initStars();
};