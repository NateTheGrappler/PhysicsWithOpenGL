#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "texture.h"
#include "Camera.h"
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <glm\detail\type_vec.hpp>
#include <map>


struct PointLight //holds the needed information for Phong lighting
{
	glm::vec3 position;
	glm::vec3 color;
	float ambient;
	float diffuse;
	float specular;
};

//a char struct for rendering text
struct Character
{
	unsigned int textureID;
	glm::ivec2   size;
	glm::ivec2   bearing;
	unsigned int advance;
};


class Renderer
{
private:
	//all of the data sent to the gpu
	unsigned int m_circleVAO,   m_circleVBO,     m_circleIBO, m_circleIndexCount;
	unsigned int m_rectVAO,     m_rectVBO,       m_rectIBO;
	unsigned int m_triangleVAO, m_triangleVBO,   m_triangleIBO;
	unsigned int m_lineVAO,     m_lineVBO;
	unsigned int m_trailVAO,     m_trailVBO;

	//3d items
	unsigned int m_sphereVAO, m_sphereVBO, m_sphereIBO, m_sphereIndexCount;
	unsigned int m_cubeVAO,   m_cubeVBO,   m_cubeIBO,   m_cubeIndexCount;

	//text
	std::map<char, Character> m_characters;
	unsigned int m_textVAO, m_textVBO;

	//shader program
	Shader m_normalShader;
	Shader m_starShader;
	Shader m_textShader;
	Shader m_trailShader;
	Shader m_lineShader;

	//matricies
	glm::mat4 m_projection;
	glm::mat4 m_view;

	bool      m_isOrtho = true;
	

public:
	Renderer();

	void init();

	//-------------------------------------FUNCTIONS DEALING WITH 2D--------------------------------------------
	
	//basic 2d shape inits (called on init of this class)
	void initCircle(int segments = 64);
	void initRectangle();
	void initTriangle();
	void initText(const std::string& fontpath);
	void initLine();

	//draw call functions that take the data and output it to the screen for 2d items
	void drawCircle(glm::vec3 position, float radius,   glm::vec3 color, glm::vec3 roation = glm::vec3(1.0f, 1.0f, 1.0f), float angle = 0, std::shared_ptr<Texture> texture = nullptr);
	void drawRect(  glm::vec3 position, glm::vec2 size, glm::vec3 color, glm::vec3 roation = glm::vec3(1.0f, 1.0f, 1.0f), float angle = 0, std::shared_ptr<Texture> texture = nullptr, glm::vec2 textureScale = glm::vec2(1.0f, 1.0f));
	void drawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color);
	void drawTriangle(glm::vec3 position, glm::vec2 size, glm::vec3 color, glm::vec3 roation = glm::vec3(1.0f, 1.0f, 1.0f), float angle = 0, std::shared_ptr<Texture> texture = nullptr);
	void drawText(std::string text, glm::vec2 position, float scale, glm::vec3 color);
	void drawGrid(int gridsize, float spacing, glm::vec3 color);
	void drawCurvedGrid(int gridsize, float spacing, std::vector<glm::vec3>& positions, std::vector<float>& magnitudes, glm::vec3 color);

	//-------------------------------------FUNCTIONS DEALING WITH 3D--------------------------------------------
	void initSphere(int sectors = 36, int stacks = 18);
	void initCube();
	//void initLine(); maybe have like a plane call to demonstrate the distorition of spacetime?
	
	void drawSphere(glm::vec3 position, float radius, glm::vec3 color, glm::vec3 rotate = glm::vec3(1.0f, 1.0f, 1.0f), float angle = 0, std::shared_ptr<Texture> texture = nullptr);
	void drawCube(glm::vec3 position, glm::vec2 size, glm::vec3 color, glm::vec3 roation = glm::vec3(1.0f, 1.0f, 1.0f), float angle = 0, std::shared_ptr<Texture> texture = nullptr);
	void drawStars(unsigned int VAO, unsigned starCount, const glm::vec3& cameraPos);

	//Additional functions
	void clear();
	void setBackgroundColor(glm::vec4 color);
	void updateMatrix(const glm::mat4& perspective, const glm::mat4& view, const glm::vec3& position);

	//shader stuff / lighting
	void setLight(PointLight& light);
	void disableLight();

	void initTrail();
	void drawTrail(std::vector<glm::vec2>& positions, glm::vec3 color);

};