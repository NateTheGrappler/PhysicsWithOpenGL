#include "Scene.h"

struct BlackHole3D
{
    glm::vec3 position;
    std::string name    = "TON618";
    std::string texture = "BlackHoleSide";
    float gridMag;   //set up to showcase the "gravity" by bending the space time grid in scene
    float radius = 2.0f;   

    float diskInner = 2.2f; //for rendering torus disk
    float diskOutter = 4.5f;

    void setRadius(float newRadius)
    {
        radius = newRadius;
    }
    void setMag(float newMag)
    {
        gridMag = newMag;
    }

    BlackHole3D(glm::vec3 position, float gridMag)
        :gridMag(gridMag), position(position)
    {

    }
};

struct lightRay3D
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 direction;

    std::vector<glm::vec3> trail;
    bool continueStep = true;
    float speed = 4.0f;

    lightRay3D(glm::vec3 position, glm::vec3 direction, glm::vec3 color)
        :position(position), direction(direction), color(color)
    {

    }

    void step(float deltaTime, const std::vector<BlackHole3D>& blackHoles)
    {
        if (checkCollision(blackHoles)) { return; }

        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;
        position.z += direction.z * speed * deltaTime;

        trail.push_back(glm::vec3(position.x, position.y, position.z));
        if (trail.size() > 50)
        {
            trail.erase(trail.begin());
        }
    }

    bool checkCollision(const std::vector<BlackHole3D>& blackHoles)
    {
        //std::cout << position.x << std::endl;

        if (!continueStep) { return true; }

        //collision with walls on x y and z axis
        if (position.x > 13 || position.x < -13) { continueStep = false;  return true;}
        if (position.y > 13 || position.y < -13) { continueStep = false;  return true;}
        if (position.z > 13 || position.z < -13) { continueStep = false;  return true;}

        //collision with any blackhole
        for (const BlackHole3D& bh : blackHoles)
        {
            //check the distance between the sphere and head point of light ray
            float dx = position.x - bh.position.x;
            float dy = position.y - bh.position.y;
            float dz = position.z - bh.position.z;

            //get the distance and compare to radus
            float distanceSquared = dx*dx + dy*dy + dz*dz;
            float radiusSquared = bh.radius * bh.radius;

            if (radiusSquared >= distanceSquared)
            {
                continueStep = false;
                return true;
            }
        }
        return false;
    }
};

struct backGroundStar
{
    glm::vec3 position;
    float     radius;
    glm::vec3 color;
    backGroundStar(glm::vec3 position, float radius, glm::vec3 color)
        :position(position), radius(radius), color(color)
    {

    }
};

class BlackHole3D_Scene : public Scene
{
private:
    std::vector<BlackHole3D>    m_blackHoles;
    std::vector<lightRay3D>     m_lightRays;
    std::vector<backGroundStar> m_bgStars;
    std::unique_ptr<Camera>     m_camera;
    PointLight                  m_pointLight;
    bool                        m_isCameraStatic = false;
    bool                        m_openGUI = false;

    //drawing the distance stars
    int m_starCount;
    unsigned int m_starVAO, m_starVBO;


public:
    BlackHole3D_Scene(Engine& m_engine);

    //all the needed functions
    void init();
    void update() override;
    void sRender() override;
    void sUserInput(const Action& action) override;
    void sGUI();
    void onEnd() override;

    void initRays(int gridSize, float spacing);
    void initStars(int count);
};