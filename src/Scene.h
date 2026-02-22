#pragma once

#include "Engine.h"
#include "Action.hpp"

typedef std::map<int, std::string> ActionMap;

class Scene
{
protected:
	Engine&   m_engine;
	ActionMap m_actionMap;
	ActionMap m_mouseActionMap;
	bool      m_paused = false;
	bool      m_hasEnded = false;

	virtual void onEnd() = 0;
	void setPaused(bool paused);

public:
	Scene(Engine& physicsEngine);
	Scene() = default;

	//functions that are required in the scene for rendering
	virtual void update() = 0;
	virtual void sRender() = 0;
	virtual void sUserInput(const Action& action) = 0;

	//TODO: functions that implement input handling
	void registerAction(int inputKey, const std::string& actionName);
	void registerMouseAction(int mouseInput, const std::string& actionName);
	const ActionMap& getActionMap() const { return m_actionMap; };
	const ActionMap& getMouseActionMap() const { return m_mouseActionMap; };


	bool hasEnded() const { return m_hasEnded; };

};