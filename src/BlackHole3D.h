#include "Scene.h"

struct BlackHole3D
{

};


class BlackHole3D_Scene : public Scene
{
private:
    std::unique_ptr<Camera>  m_camera;
    PointLight               m_pointLight;


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