#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include <glm/glm.hpp>

class Light
{
public:
	struct Attenuation {
		float m_constant{};
		float m_linear{};
		float m_quadratic{};


	};

	//constructor for point light
	Light(glm::vec3 position, Attenuation attenuation)
		:m_position{position}, m_attenuation{attenuation}
	{
		std::cout << "Point Light made!\n";
	}
	//constructor for directional light
	Light(glm::vec3 direction)
		:m_direction{direction}
	{
		std::cout << "Directional Light made!\n";
	}
	//constructor for spot light
	Light(glm::vec3 position, glm::vec3 direction, Attenuation& attenuation, float cutOff)
		:m_position{ position },
		m_direction{ direction },
		m_attenuation{ attenuation },
		m_cutOff{ cutOff }
	{
		std::cout << "Spot Light made!\n";
	}

	void setPosition(glm::vec3 pos)
	{
		m_position = pos;
	}

	glm::vec3& getPosition()
	{
		return m_position;
	}

	glm::vec3 getDirection() const
	{
		return m_direction;
	}

	float getCutOff() const
	{
		return m_cutOff;
	}

	float getSpecularity() const
	{
		return m_specularity;
	}

	Attenuation getLightAttenuation() const
	{
		return m_attenuation;
	}

private:
	glm::vec3 m_position{};
	glm::vec3 m_direction{};
	Attenuation m_attenuation{};

	float m_cutOff{};
	float m_specularity{};
};

#endif