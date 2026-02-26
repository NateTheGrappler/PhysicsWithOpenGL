#pragma once
#include "Scene.h"
#include "cmath"


struct blackHole2D
{
    double r_s; //the event horizon (Schwarzschild radius_
    double mass;
    glm::vec3 position;
    glm::vec3 color;
    double renderScale;


    //function to init struct
    blackHole2D(double mass, glm::vec3 position, glm::vec3 color, double pixelRadius)
        :mass(mass), position(position), color(color)
    {
        calculateR_S(299792458, 6.67 * std::pow(10, -11));
        calculateRenderScale(pixelRadius);
    }


    void calculateRenderScale(double pixelRadius) { renderScale = pixelRadius / r_s; }
    float getRenderRadius() const { return static_cast<float>(r_s * renderScale); }
    void calculateR_S(double speedofLight, double G) { r_s = (2 * G * mass) / (speedofLight * speedofLight); }
};

struct lightRay
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 direction;

    std::vector<glm::vec2> trail;

    lightRay(glm::vec3 pos, glm::vec3 color, glm::vec2 dir)
        :position(pos), color(color), direction(dir)
    { }

    void step(double renderScale, float deltaTime)
    {
        //calculate position based on light speed and direction
        position.x += direction.x * 299792458 * renderScale * deltaTime;
        position.y += direction.y * 299792458 * renderScale * deltaTime;

        //update the trail vector with the trailing positions
        trail.push_back(glm::vec2(position.x, position.y));

        if (trail.size() > 500)
        {
            trail.erase(trail.begin()); // get rid of oldest point
        }
    }

};

class BlackHole2D_Scene : public Scene
{
private:
    std::vector<blackHole2D> m_blackHoles;
    std::vector<lightRay>    m_lightRays;
    double m_lightSpeed = 299792458;
    double m_GConstant = 6.67 * std::pow(10, -11);
    double m_renderScale = 1e-6;   //to scale things down from meters to pixels

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