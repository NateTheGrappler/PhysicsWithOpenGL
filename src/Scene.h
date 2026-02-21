#pragma once

#include "Engine.h"


class Scene
{
protected:
	Engine& m_engine;
	bool    m_paused = false;
	bool    m_hasEnded = false;

	virtual void onEnd() = 0;
	void setPaused(bool paused);

public:
	Scene(Engine& physicsEngine);
	Scene() = default;

	//functions that are required in the scene for rendering
	virtual void update() = 0;
	virtual void sRender() = 0;

	//TODO: functions that implement input handling


	bool hasEnded() const { return m_hasEnded; };

};