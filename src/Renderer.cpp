#include "Renderer.h"

Renderer::Renderer()
	:m_normalShader("src/res/shader/basic.shader"), m_camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
	init();
}

void Renderer::init()
{
	//set up all of the needed inner information for the 2d shapes
	initCircle();
	initRectangle();
	initLine();

	initCube();
	initSphere();

	m_projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


	//init buffer dept
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


//---------------------------Setting up 2D shapes------------------------------------

void Renderer::initCircle(int segments)
{
	std::vector<float> verticies;
	std::vector<unsigned int> indicies;

	//center point
	verticies.push_back(0.0f); // x
	verticies.push_back(0.0f); // y
	verticies.push_back(0.0f); // z
	verticies.push_back(0.0f); //normal x
	verticies.push_back(0.0f); //normal y
	verticies.push_back(1.0f); //normal z
	verticies.push_back(0.5f); // u
	verticies.push_back(0.5f); // v

	//ring verticies
	for (int i = 0; i <= segments; i++)
	{
		float angle = 2.0f * glm::pi<float>() * i / segments;
		float x = cos(angle);
		float y = sin(angle);

		verticies.push_back(x); // x
		verticies.push_back(y); // y
		verticies.push_back(0.0f); // z
		verticies.push_back(0.0f); //normal x
		verticies.push_back(0.0f); //normal y
		verticies.push_back(1.0f); //normal z
		verticies.push_back(x * 0.5f + 0.5f); // u
		verticies.push_back(y * 0.5f + 0.5f); // v
	}

	//set up the order for the indicies so the triangles connect
	for (int i = 1; i <= segments; i++)
	{
		indicies.push_back(0);   //center point (first in array);
		indicies.push_back(i);   //corresponding outer ring vertex
		indicies.push_back(i+1); //the next index in order
	}

	//internal var to keep track of indice size
	m_circleIndexCount = indicies.size();


	//asign the data to the inner class values
	glGenVertexArrays(1, &m_circleVAO);
	glBindVertexArray(m_circleVAO);

	glGenBuffers(1, &m_circleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_circleVBO);
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_circleIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_circleIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(float), indicies.data(), GL_STATIC_DRAW);

	//assign the attribute of the data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
   
}

void Renderer::initRectangle()
{
	//set verticies
	float vertieces[] = 
	{
	  //position coordinates       //normals          //texture coordinates
		 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,         0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,         1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,         0.0f, 1.0f
	};

	unsigned int indicies[] =
	{
		0, 1, 2, //first triangle
		2, 3, 0, //second triangle
	};

	//asign the data to the inner class values
	glGenVertexArrays(1, &m_rectVAO);
	glBindVertexArray(m_rectVAO);

	glGenBuffers(1, &m_rectVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertieces), vertieces, GL_STATIC_DRAW);

	glGenBuffers(1, &m_rectIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rectIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	//assign the attribute of the data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

}

void Renderer::initLine()
{

}


//---------------------------Drawing 2D shapes------------------------------------

void Renderer::drawCircle(glm::vec3 position, float radius, glm::vec3 color, glm::vec3 rotate, float angle, Texture* texture)
{
	//use the shader
	m_normalShader.use();

	//check to see if a texture was passed, and if so tell the shader to implement it
	if (texture)
	{
		texture->bind(0);
		m_normalShader.setUniformInt("useTexture", 1);
		m_normalShader.setUniformInt("tex", 0);
	}
	else
	{
		m_normalShader.setUniformInt("useTexture", 0);
	}

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(radius, radius, 1.0f));
	model = glm::rotate(model, angle, rotate);

	//toss all of this information over to the shader
	m_normalShader.setMat4("model", model);
	m_normalShader.setMat4("view", m_view);
	m_normalShader.setMat4("projection", m_projection);
	m_normalShader.setVec3("color", color);

	//make the actual draw call
	glBindVertexArray(m_circleVAO);
	glDrawElements(GL_TRIANGLES, m_circleIndexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Renderer::drawRect(glm::vec3 position, glm::vec2 size, glm::vec3 color, glm::vec3 rotate, float angle, Texture* texture)
{
	//use the shader
	m_normalShader.use();

	//check to see if a texture was passed, and if so tell the shader to implement it
	if (texture)
	{
		texture->bind(0);
		m_normalShader.setUniformInt("useTexture", 1);
		m_normalShader.setUniformInt("tex", 0);
	}
	else
	{
		m_normalShader.setUniformInt("useTexture", 0);
	}

	//set up matrix stuff
	//m_projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	m_view = glm::mat4(1.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
	model = glm::scale    (model, glm::vec3(size.x, size.y, 1.0f));
	model = glm::rotate(model, angle, rotate);

	//toss all of this information over to the shader
	m_normalShader.setMat4("model", model);
	m_normalShader.setMat4("view", m_view);
	m_normalShader.setMat4("projection", m_projection);
	m_normalShader.setVec3("color", color);

	//make the actual draw call
	glBindVertexArray(m_rectVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Renderer::drawLine(glm::vec2 start, glm::vec2(), glm::vec3 color, Texture* texture)
{

}


//---------------------------Setting up 3D shapes------------------------------------

void Renderer::initSphere(int sectors, int stacks)
{
	std::vector<float> verticies;
	std::vector<unsigned int> indicies;

	for (int i = 0; i <= stacks; i++)
	{
		//goes from pi/2 (top) to -pi/2 (bottom)
		float stackAngle = glm::pi<float>() / 2.0f - i * (glm::pi<float>() / stacks);
		float xy = cos(stackAngle);
		float z  = sin(stackAngle);

		for (int j = 0; j <= sectors; j++)
		{
			float sectorAngle = j * (2.0f * glm::pi<float>() / sectors);

			float x = xy * cos(sectorAngle);
			float y = xy * sin(sectorAngle);

			//position
			verticies.push_back(x);
			verticies.push_back(y);
			verticies.push_back(z);
			//normals
			verticies.push_back(x);
			verticies.push_back(y);
			verticies.push_back(z);
			//texture
			verticies.push_back((float)j / sectors);
			verticies.push_back((float)i / stacks);


		}
	}

	for (int i = 0; i < stacks; i++)
	{
		int k1 = i * (sectors + 1);
		int k2 = k1 + (sectors + 1);

		for (int j = 0; j < sectors; j++, k1++, k2++)
		{
			if (i != 0)
			{
				indicies.push_back(k1);
				indicies.push_back(k2);
				indicies.push_back(k1 + 1);
			}
			if (i != stacks - 1)
			{
				indicies.push_back(k1 + 1);
				indicies.push_back(k2);
				indicies.push_back(k2 + 1);
			}
		}
	}

	m_sphereIndexCount = indicies.size();

	glGenVertexArrays(1, &m_sphereVAO);
	glBindVertexArray(m_sphereVAO);

	glGenBuffers(1, &m_sphereVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_sphereIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), indicies.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Renderer::initCube()
{
	float verticies[] =
	{
		//position                //normals         //textures
		// Front face
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,    0.0f, 0.0f, // 0
		 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,    1.0f, 0.0f, // 1
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,    1.0f, 1.0f, // 2
		-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,    0.0f, 1.0f, // 3

		// Back face
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f, // 4
		 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f, // 5
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f, // 6
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f, // 7

		// Left face
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,    0.0f, 0.0f, // 8
		-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,    1.0f, 0.0f, // 9
		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,    1.0f, 1.0f, // 10
		-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,    0.0f, 1.0f, // 11

		// Right face
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,    1.0f, 0.0f, // 12
		 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,    0.0f, 0.0f, // 13
		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,    0.0f, 1.0f, // 14
		 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,    1.0f, 1.0f, // 15

		 // Bottom face
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,    0.0f, 1.0f, // 16
		  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,    1.0f, 1.0f, // 17
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    1.0f, 0.0f, // 18
 		 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    0.0f, 0.0f, // 19
 
		 // Top face
		 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,    0.0f, 0.0f, // 20
		  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,    1.0f, 0.0f, // 21
		  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    1.0f, 1.0f, // 22
		 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    0.0f, 1.0f  // 23
	};

	unsigned int indicies[] =
	{
		0,  1,  2,   2,  3,  0,  // front
		4,  6,  5,   6,  4,  7,  // back
		8,  9,  10,  10, 11, 8,  // left
		12, 13, 14,  14, 15, 12, // right
		16, 17, 18,  18, 19, 16, // top
		20, 21, 22,  22, 23, 20, // bottom
	};

	m_cubeIndexCount = 36;

	//asign the data to the inner class values
	glGenVertexArrays(1, &m_cubeVAO);
	glBindVertexArray(m_cubeVAO);

	glGenBuffers(1, &m_cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glGenBuffers(1, &m_cubeIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	//assign the attribute of the data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);


}


//---------------------------Drawing 3D shapes------------------------------------
void Renderer::drawSphere(glm::vec3 position, float radius, glm::vec3 color, glm::vec3 rotate, float angle, Texture* texture)
{
	//use the shader
	m_normalShader.use();

	//check to see if a texture was passed, and if so tell the shader to implement it
	if (texture)
	{
		texture->bind(0);
		m_normalShader.setUniformInt("useTexture", 1);
		m_normalShader.setUniformInt("tex", 0);
	}
	else
	{
		m_normalShader.setUniformInt("useTexture", 0);
	}

	//set up matrix stuff
	//m_projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	m_view = glm::mat4(1.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
	model = glm::scale(model, glm::vec3(radius, radius, radius));
	model = glm::rotate(model, angle, rotate);

	//toss all of this information over to the shader
	m_normalShader.setMat4("model", model);
	m_normalShader.setMat4("view", m_view);
	m_normalShader.setMat4("projection", m_projection);
	m_normalShader.setVec3("color", color);

	//make the actual draw call
	glBindVertexArray(m_sphereVAO);
	glDrawElements(GL_TRIANGLES, m_sphereIndexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Renderer::drawCube(glm::vec3 position, glm::vec2 size, glm::vec3 color, glm::vec3 rotate, float angle, Texture* texture)
{
	//use the shader
	m_normalShader.use();

	//check to see if a texture was passed, and if so tell the shader to implement it
	if (texture)
	{
		texture->bind(0);
		m_normalShader.setUniformInt("useTexture", 1);
		m_normalShader.setUniformInt("tex", 0);
	}
	else
	{
		m_normalShader.setUniformInt("useTexture", 0);
	}

	//set up matrix stuff
	//m_projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	m_view = glm::mat4(1.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
	model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));
	model = glm::rotate(model, angle, rotate);

	//toss all of this information over to the shader
	m_normalShader.setMat4("model", model);
	m_normalShader.setMat4("view", m_view);
	m_normalShader.setMat4("projection", m_projection);
	m_normalShader.setVec3("color", color);

	//make the actual draw call
	glBindVertexArray(m_cubeVAO);
	glDrawElements(GL_TRIANGLES, m_cubeIndexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}





void Renderer::setBackgroundColor(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);

}

void Renderer::updateCameraView()
{
	m_view = m_camera.getViewMatrix();
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
