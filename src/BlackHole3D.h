#include "Scene.h"

struct BlackHole3D
{
    glm::vec3 position;
    std::string name    = "TON618";
    std::string texture = "BlackHoleSide";
    float gridMag;   //set up to showcase the "gravity" by bending the space time grid in scene

    BlackHole3D(glm::vec3 position, float gridMag)
        :gridMag(gridMag), position(position)
    {

    }
};


class BlackHole3D_Scene : public Scene
{
private:
    std::vector<BlackHole3D> m_blackHoles;
    std::unique_ptr<Camera>  m_camera;
    PointLight               m_pointLight;
    bool                     m_isCameraStatic = false;
    bool                     m_openGUI = false;


public:
    BlackHole3D_Scene(Engine& m_engine);

    //all the needed functions
    void init();
    void update() override;
    void sRender() override;
    void sUserInput(const Action& action) override;
    void sGUI();
    void onEnd() override;
};