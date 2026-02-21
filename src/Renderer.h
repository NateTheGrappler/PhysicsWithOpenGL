#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <vector>

class Renderer
{
private:
	//all of the data sent to the gpu
	unsigned int m_circleVAO, m_circleVBO, m_circleIBO;
	unsigned int m_rectVAO,   m_rectVBO,   m_rectIBO;
	unsigned int m_lineVAO,   m_lineVBO,   m_lineIBO;

	//shader program

public:
	Renderer();

	//basic 2d shape inits (called on init of this class)
	void initCircle();
	void initRectangle();
	void initLine();

	//draw call functions that take the data and output it to the screen for 2d items
	void drawCircle();
	void drawRect();
	void drawLine();
};