#pragma once

#include "Scene.h"
#include <map>
#include <memory>

class Scene_Menu : public Scene
{
public:
    Scene_Menu(Engine& gameEngine);

    void init();
    void update() override;
    void sRender() override;
    void onEnd() override;
};