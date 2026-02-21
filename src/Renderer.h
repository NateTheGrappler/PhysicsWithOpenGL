#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "texture.h"

#include <vector>
#include <glm\detail\type_vec.hpp>

class Renderer
{
private:
	//all of the data sent to the gpu
	unsigned int m_circleVAO, m_circleVBO, m_circleIBO;
	unsigned int m_rectVAO,   m_rectVBO,   m_rectIBO;
	unsigned int m_lineVAO,   m_lineVBO,   m_lineIBO;

	//shader program
	Shader m_normalShader;

	//matricies
	glm::mat4 m_projection;
	glm::mat4 m_view;
	

public:
	Renderer();

	void init();

	//basic 2d shape inits (called on init of this class)
	void initCircle();
	void initRectangle();
	void initLine();

	//draw call functions that take the data and output it to the screen for 2d items
	void drawCircle(glm::vec2 position, float radius,   glm::vec3 color, Texture* texture = nullptr);
	void drawRect(  glm::vec2 position, glm::vec2 size, glm::vec3 color, Texture* texture = nullptr);
	void drawLine(  glm::vec2 start,    glm::vec2(),    glm::vec3 color, Texture* texture = nullptr);

	//background functions
	void clear();
	void setBackgroundColor(glm::vec4 color);
};