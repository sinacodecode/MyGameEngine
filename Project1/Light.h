#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include <glm/glm.hpp>

class Light
{
public:
	struct Attenuation {
		float m_constant{ 1.0f };
		float m_linear{ 0.09f };
		float m_quadratic{ 0.032f };


	};

	//constructor for point light
	Light(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), Attenuation attenuation = {1.0f , 0.09f, 0.032f})
		:m_position{position}, m_attenuation{attenuation}
	{
		std::cout << "Point Light made!\n";
	}
	//constructor for directional light
	//Light(glm::vec3 direction)
	//	:m_direction{direction}
	//{
	//	std::cout << "Directional Light made!\n";
	//}
	//constructor for spot light
	//Light(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 direction, Attenuation attenuation = { 1.0f , 0.09f, 0.032f }, float cutOff)
	//	:m_position{ position },
	//	m_direction{ direction },
	//	m_attenuation{ attenuation },
	//	m_cutOff{ cutOff }
	//{
	//	std::cout << "Spot Light made!\n";
	//}

	void setPosition(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f))
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

	float& getSpecularity()
	{
		return m_specularity;
	}

	Attenuation& getLightAttenuation()
	{
		return m_attenuation;
	}

	glm::vec3 getAmbient() const 
	{
		return m_ambient;
	}

	glm::vec3& getAmbientRef()
	{
		return m_ambient;
	}

	glm::vec3 getDiffuse() const 
	{
		return m_diffuse;
	}

	glm::vec3& getDiffuseRef()
	{
		return m_diffuse;
	}

	glm::vec3 getSpecular() const
	{
		return m_specular;
	}

	glm::vec3& getSpecularRef()
	{
		return m_specular;
	}

private:
	glm::vec3 m_position{};
	glm::vec3 m_direction{};
	Attenuation m_attenuation{ 1.0f , 0.09f, 0.032f};

	glm::vec3 m_diffuse {1.0f, 0.8f, 0.5f};
	glm::vec3 m_ambient {0.4f, 0.4f, 0.8f};
	glm::vec3 m_specular{ 1.0f, 1.0f, 1.0f };
	float m_cutOff{};
	float m_specularity{};
};

#endif