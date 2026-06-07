#ifndef LIGHT_H
#define LIGHT_H

#include <variant>
#include <glm/glm.hpp>

namespace Light
{
	struct Attenuation
	{
		float constant{ 1.0f };
		float linear{ 0.09f };
		float quadratic{ 0.032f };
	};

	struct Color
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct PointLight
	{
		Color color;

		glm::vec3 position;

		Attenuation attenuation;
	};

	struct DirectionalLight
	{
		Color color;

		glm::vec3 direction;
	};

	struct SpotLight
	{
		Color color;

		glm::vec3 position;
		glm::vec3 direction;
		Attenuation attenuation;

		float cutOff;
		float outerCutOff;
		//float cutOff{ glm::cos(glm::radians(12.5f)) };
		//float outerCutOff{ glm::cos(glm::radians(15.0f)) };
	};

	using LightVariant = std::variant< PointLight, DirectionalLight, SpotLight> ;
};

#endif