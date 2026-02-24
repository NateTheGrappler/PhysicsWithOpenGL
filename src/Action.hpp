#pragma once
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Action
{
	std::string m_name = "NONE";
	std::string m_type = "NONE";
	glm::vec2   m_pos;

public:

	Action() = default;

	Action(const std::string& name, const std::string& type, const glm::vec2& m_pos = glm::vec2(0, 0))
		: m_name(name)
		, m_type(type) 
		, m_pos(m_pos)
	{
	}

	const std::string& name() const
	{
		return m_name;
	}
	const std::string& type() const
	{
		return m_type;
	}
	const glm::vec2& pos() const
	{
		return m_pos;
	}
};