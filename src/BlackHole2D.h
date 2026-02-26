#pragma once
#include "Scene.h"
#include "cmath"


struct blackHole2D
{
    double r_s; //the event horizon (Schwarzschild radius_
    double mass;
    glm::vec3 position;
    glm::vec3 color;


    //function to init struct
    blackHole2D(double mass, glm::vec3 position, glm::vec3 color)
        :mass(mass), position(position), color(color)
    {
        calculateR_S(299792458, 6.67 * std::pow(10, -11));
    }

    void calculateR_S(double speedofLight, double G) { r_s = (2 * G * mass) / (speedofLight * speedofLight); }
};

struct lightRay
{
    glm::vec3 position;
    glm::vec3 color;
    double velocity;
};

class BlackHole2D_Scene : public Scene
{
private:
    std::vector<blackHole2D> m_blackHoles;
    double m_lightSpeed = 299792458;
    double m_GConstant = 6.67 * std::pow(10, -11);

    bool                     m_cameraIsStatic = true;
    std::unique_ptr<Camera>  m_camera;



public:
    BlackHole2D_Scene(Engine& gameEngine);

    //inherited functions
    void init();
    void update() override;
    void sRender() override;
    void sUserInput(const Action& action) override;
    void onEnd() override;
};