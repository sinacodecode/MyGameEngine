#ifndef SCENE_H
#define SCENE_H

#include "SceneObject.h"
#include "Light.h"

#include <vector>

class Scene
{
public:
	Scene(std::vector<Object> objects, std::vector<Light> lights)
		:m_objects{objects}, m_lights{lights}
	{
	}
	Scene(Scene&) = delete;

	std::vector<Object> getObjects() const { return m_objects; }
	std::vector<Light> getLights() const { return m_lights; }
private:
	std::vector<Object> m_objects{};
	std::vector<Light> m_lights{};
};

#endif