#include "Renderer.h"

Renderer::Renderer()
	:m_normalShader("src/res/shader/basic.shader"),
	m_starShader("src/res/shader/starShader.shader"),
	m_textShader("src/res/shader/text.shader"),
	m_trailShader("src/res/shader/trail.shader"),
	m_lineShader("src/res/shader/line.shader")
{
	init();
}

void Renderer::init()
{
	//set up all of the needed inner information for the 2d shapes
	initCircle();
	initRectangle();
	initTriangle();
	initLine();
	initText("src/res/fonts/arialnarrow.ttf");
	initTrail();
	initCube();
	initSphere();
	initGrid();

	//for rendering stars in menu
	glEnable(GL_PROGRAM_POINT_SIZE);

	//init buffer dept
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
		//position                 //normals            //texture coordinates
		   0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,  // top right
		   0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f,  // bottom right
		  -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  // bottom left
		  -0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f   // top left
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
void Renderer::initTriangle()
{
	//set verticies
	float vertieces[] =
	{
	   //position coordinates       //normals          //texture coordinates
		-0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f,         0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f,         1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f,         0.5f, 1.0f,
	};

	unsigned int indicies[] =
	{
		0, 1, 2, //first triangle
	};

	//asign the data to the inner class values
	glGenVertexArrays(1, &m_triangleVAO);
	glBindVertexArray(m_triangleVAO);

	glGenBuffers(1, &m_triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertieces), vertieces, GL_STATIC_DRAW);

	glGenBuffers(1, &m_triangleIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triangleIBO);
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
void Renderer::initText(const std::string& fontpath)
{
	//load in the free type library and set up the font
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "failed to load in free type library" << std::endl;
	}
	FT_Face face;
	if (FT_New_Face(ft, fontpath.c_str(), 0, &face))
	{
		std::cout << "Failed to load font: " << fontpath << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, 48); //the zero here allows for the width to get calculated from the height
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //disable byte allignement bc opengl needs 4 bytes per texture or something


	//load all 128 chars
	for (unsigned char c = 0; c < 128; c++) //(get it)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "failed to load character: " << c << std::endl;
		}

		//load in all of the needed texture data for open gl
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		//set tex params
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//save data to struct and upload to render class
		Character character =
		{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x
		};

		m_characters.insert(std::pair<char, Character>(c, character));
	}

	//free the memory from facetype
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	//load memory in opengl
	glGenVertexArrays(1, &m_textVAO);
	glBindVertexArray(m_textVAO);
	glGenBuffers(1, &m_textVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW); //dynamic is used here because vertices change based on character
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}
void Renderer::initGrid()
{
	glGenVertexArrays(1, &m_gridVAO);
	glBindVertexArray(m_gridVAO);

	glGenBuffers(1, &m_gridVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_gridVBO);
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW); //empty buffer allows for dynamic grid size

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}
void Renderer::initLine()
{
	glGenVertexArrays(1, &m_lineVAO);
	glBindVertexArray(m_lineVAO);

	glGenBuffers(1, &m_lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}


//---------------------------Drawing 2D shapes------------------------------------

void Renderer::drawCircle(glm::vec3 position, float radius, glm::vec3 color, glm::vec3 rotate, float angle, std::shared_ptr<Texture> texture)
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	glDisable(GL_BLEND);
	glBindVertexArray(0);
}
void Renderer::drawRect(glm::vec3 position, glm::vec2 size, glm::vec3 color, glm::vec3 rotate, float angle, std::shared_ptr<Texture> texture, glm::vec2 textureScale)
{
	//use the shader
	m_normalShader.use();
	m_normalShader.setVec2("uvScale", textureScale);

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
void Renderer::drawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color)
{
	float vertices[] =
	{
		start.x, start.y, start.z,
		end.x,   end.y,   end.z
	};

	m_lineShader.use();
	m_lineShader.setMat4("model", glm::mat4(1.0f));
	m_lineShader.setMat4("view", m_view);
	m_lineShader.setMat4("projection", m_projection);
	m_lineShader.setVec3("color", color);

	glBindVertexArray(m_lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);


}
void Renderer::drawTriangle(glm::vec3 position, glm::vec2 size, glm::vec3 color, glm::vec3 rotate, float angle, std::shared_ptr<Texture> texture)
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
	glBindVertexArray(m_triangleVAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
void Renderer::drawText(std::string text, glm::vec2 position, float scale, glm::vec3 color)
{
	//use text and send all uniforms to shader
	m_textShader.use();
	m_textShader.setVec3("textColor", color);
	m_textShader.setMat4("projection", m_projection);

	//activate blending in open gl
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_textVAO);

	for (const char& c : text)
	{
		Character character = m_characters[c];

		float xpos   = position.x + character.bearing.x * scale;
		float ypos   = position.y - (character.size.y - character.bearing.y) * scale;
		float width  = character.size.x * scale;
		float height = character.size.y * scale;

		float vertices[6][4] =
		{
			{xpos,         ypos + height, 0.0f, 0.0f},
			{xpos,         ypos,          0.0f, 1.0f},
			{xpos + width, ypos,          1.0f, 1.0f},
			{xpos,         ypos + height, 0.0f, 0.0f},
			{xpos + width, ypos,          1.0f, 1.0f},
			{xpos + width, ypos + height, 1.0f, 0.0f}
		};

		glBindTexture(GL_TEXTURE_2D, character.textureID);
		glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//shift to the right bu 6 to get the pixels
		position.x += (character.advance >> 6) * scale;
	}

	//unbind data
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Renderer::drawGrid(int gridSize, float spacing, glm::vec3 color)
{
	float halfsize = gridSize * spacing / 2;

	//x axis
	for (int i = 0; i <= gridSize; i++)
	{
		float pos = -halfsize + i * spacing;
		drawLine(glm::vec3(-halfsize, 0.0f, pos), glm::vec3(halfsize, 0.0f, pos), color);
	}

	//z axis
	for (int i = 0; i <= gridSize; i++)
	{
		float pos = -halfsize + i * spacing;
		drawLine(glm::vec3(pos, 0.0f, -halfsize), glm::vec3(pos, 0.0f, halfsize), color);
	}
}

void Renderer::drawCurvedGrid(int gridSize, float spacing, std::vector<glm::vec3>& positions, std::vector<float>& magnitudes, glm::vec3 color)
{

	//function to get the y displacement that occures between the grid and the mass objects based on the magnitude
	auto calcY = [&](float x, float z) -> float
		{
			float totalDisplacement = 0.0f;
			for (int i = 0; i < positions.size(); i++)
			{
				float dx = x - positions[i].x;
				float dz = z - positions[i].z;
				float dist = sqrt(dx * dx + dz * dz);
				totalDisplacement -= magnitudes[i] / (dist + 0.5f);
			}
			return totalDisplacement;
		};


	float halfsize = gridSize * spacing / 2;
	
	std::vector<float> verticies;
	verticies.reserve(gridSize * (gridSize + 1) * 2 * 2 * 3); //allocate the space based on 2 verticies in line with 3 floats each

	//x lines
	for (int i = 0; i <= gridSize; i++)
	{
		float z = -halfsize + i * spacing;
		for(int j = 0; j <= gridSize; j++)
		{
			//calculate the start and end points for the line
			float x1 = -halfsize + j * spacing;
			float x2 = -halfsize + (j + 1) * spacing;

			//push both points back at the given z axis in the grid, alongside the displaced y
			verticies.push_back(x1);
			verticies.push_back(calcY(x1, z));
			verticies.push_back(z);

			verticies.push_back(x2);
			verticies.push_back(calcY(x2, z));
			verticies.push_back(z);
		}
	}

	//z lines
	for (int i = 0; i <= gridSize; i++)
	{
		float x = -halfsize + i * spacing;
		for (int j = 0; j <= gridSize; j++)
		{
			float z1 = -halfsize + j * spacing;
			float z2 = -halfsize + (j + 1) * spacing;

			verticies.push_back(x);
			verticies.push_back(calcY(x, z1));
			verticies.push_back(z1);

			verticies.push_back(x);
			verticies.push_back(calcY(x, z2));
			verticies.push_back(z2);
		}
	}


	//draw call to render the actual grid that was set up
	m_lineShader.use();
	m_lineShader.setMat4("model", glm::mat4(1.0f));
	m_lineShader.setMat4("view", m_view);
	m_lineShader.setMat4("projection", m_projection);
	m_lineShader.setVec3("color", color);

	glBindVertexArray(m_gridVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_gridVBO);
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_LINES, 0, verticies.size() / 3);
	glBindVertexArray(0);
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
void Renderer::drawSphere(glm::vec3 position, float radius, glm::vec3 color, glm::vec3 rotate, float angle, std::shared_ptr<Texture> texture)
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
void Renderer::drawCube(glm::vec3 position, glm::vec2 size, glm::vec3 color, glm::vec3 rotate, float angle, std::shared_ptr<Texture> texture)
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
void Renderer::drawStars(unsigned int VAO, unsigned starCount, const glm::vec3& cameraPos)
{
	m_starShader.use();

	glm::mat4 model = glm::mat4(1.0f);

	m_normalShader.setMat4("model", model);
	m_normalShader.setMat4("view", m_view);
	m_normalShader.setMat4("projection", m_projection);

	glDepthMask(GL_FALSE);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, starCount);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}



//-------------------------------Asthetics------------------------------------

void Renderer::setBackgroundColor(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);

}
void Renderer::updateMatrix(const glm::mat4& perspective, const glm::mat4& view, const glm::vec3& position)
{
	m_projection = perspective;
	m_view = view;

	//lighting stuff for shader
	m_normalShader.use();
	m_normalShader.setVec3("viewPos", position);
}
void Renderer::setLight(PointLight& light)
{
	//set inner shader uniforms to be used for light calculation
	m_normalShader.use();
	m_normalShader.setVec3		  ("lightPos",		   light.position);
	m_normalShader.setVec3		  ("lightColor",       light.color);
	m_normalShader.setUniformFloat("ambientStrength",  light.ambient);
	m_normalShader.setUniformFloat("diffuseStrength",  light.diffuse);
	m_normalShader.setUniformFloat("specularStrength", light.specular);

}
void Renderer::disableLight()
{
	m_normalShader.use();
	m_normalShader.setUniformFloat("ambientStrength",  1.0f);
	m_normalShader.setUniformFloat("diffuseStrength",  0.0f);
	m_normalShader.setUniformFloat("specularStrength", 0.0f);
}

void Renderer::initTrail()
{
	glGenVertexArrays(1, &m_trailVAO);
	glBindVertexArray(m_trailVAO);

	glGenBuffers(1, &m_trailVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_trailVBO);
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
void Renderer::drawTrail(std::vector<glm::vec2>& positions, glm::vec3 color)
{
	m_trailShader.use();
	m_trailShader.setMat4("projection", m_projection);

	std::vector<float> vertices;

	if (positions.empty()) { return; }

	for (int i = 0; i < positions.size(); i++)
	{
		float alpha = (float)i / positions.size();

		vertices.push_back(positions[i].x);
		vertices.push_back(positions[i].y);
		vertices.push_back(0.0f);
		vertices.push_back(color.r);
		vertices.push_back(color.g);
		vertices.push_back(color.b);
		vertices.push_back(alpha);
	}

	glBindVertexArray(m_trailVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_trailVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(2.0f);
	glDrawArrays(GL_LINE_STRIP, 0, positions.size());

	glDisable(GL_BLEND);
	glBindVertexArray(0);
}



void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
