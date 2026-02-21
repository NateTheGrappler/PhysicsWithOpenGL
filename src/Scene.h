#include "Engine.h"


class Scene
{
protected:
	Engine& m_engine;
	bool    m_paused = false;
	bool    m_hasEnded = false;

public:
	Scene(Engine& physicsEngine);

	//functions that are required in the scene for rendering
	virtual void update() = 0;
	virtual void sRender() = 0;

	//TODO: functions that implement input handling


	bool hasEnded() const { return m_hasEnded; };
	void setPaused(bool trueOrFalse);


};