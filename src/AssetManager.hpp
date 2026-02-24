#pragma once

#include "texture.h"
#include <memory>
#include <unordered_map>

class AssetManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

public:
	void loadTexture(const std::string& name, const std::string& filepath)
	{
		if (m_textures.count(name)) return;
		m_textures[name] = std::make_shared<Texture>(filepath);
	}

	std::shared_ptr<Texture> getTexture(const std::string& name)
	{
		return m_textures.at(name);
	}
};