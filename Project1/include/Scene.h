#ifndef SCENE_H
#define SCENE_H

#include "SceneObject.h"
#include "Light.h"

#include <vector>

class Scene
{
public:
	Scene(Camera& camera, std::vector<std::unique_ptr<Object>> objects, std::vector< std::unique_ptr<Light::LightVariant>> lights)
		:m_camera{camera},m_objects {std::move(objects)}, m_lights{std::move(lights)}
	{
	}
	Scene(Scene&) = delete;
	Scene& operator = (Scene&) = delete;

	void pushObject(std::unique_ptr<Object> object)
	{
		m_objects.emplace_back(std::move(object));
	}
	void popObject()
	{
		if(!m_objects.empty())
			m_objects.pop_back();
		std::cout << m_objects.size() << " objects left in scene\n";
	}

	void pushLight(std::unique_ptr<Light::LightVariant> light)
	{
		if (light->index() == 0)
		{
			m_pointLightCount++;
			std::cout << "Point light added. Remaining point lights: " << m_pointLightCount << '\n';
		}
		m_lights.emplace_back(std::move(light));
	}
	void popLights()
	{
		if (!m_lights.empty())
			m_lights.pop_back();
		std::cout << m_lights.size() << " objects left in scene\n";
	}

	void removeLightAt(size_t index)
	{
		if(index < m_lights.size())
		if (m_lights.at(index)->index() == 0)
		{
			m_pointLightCount--;
			std::cout << "Point light removed. Remaining point lights: " << m_pointLightCount << '\n';
		}
		m_lights.erase(m_lights.begin() + static_cast<std::vector<std::unique_ptr<Light::LightVariant>>::difference_type>(index));
	}

	int m_pointLightCount{};
	std::vector< std::unique_ptr<Object>>& getSceneObjects() { return m_objects; }
	std::vector< std::unique_ptr<Light::LightVariant>>& getSceneLights() { return m_lights; }
	Camera& getSceneCamera() { return m_camera; }
private:
	std::vector<std::unique_ptr<Object>> m_objects{};
	std::vector<std::unique_ptr<Light::LightVariant>> m_lights{};
	Camera& m_camera;

};

#endif