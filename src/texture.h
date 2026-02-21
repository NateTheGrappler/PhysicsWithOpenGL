#pragma once
#include <string>
#include "glad\glad.h"


class Texture
{
private:
	unsigned int m_renderID;
	int m_width, m_height, m_nrChannels;
	const std::string m_texturePath;

public:
	Texture(const std::string& filePath);
	~Texture();

	void bind() const;
	void bind(unsigned int unit) const;
	void unbind() const;

	void setParameter(int pname, int param);

	inline int getWidth() const { return m_width; };
	inline int getHeight() const { return m_height; };
	inline int getnrChannels() const { return m_nrChannels; };
	inline const unsigned int getID() const { return m_renderID; };
	inline const std::string getTexturePath() const& { return m_texturePath; };

};