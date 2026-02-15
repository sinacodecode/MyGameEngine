#ifndef SCENE_H
#define SCENE_H

#include "SceneObject.h"
#include "Light.h"

#include <vector>

class Scene
{
	Scene(std::vector<Object> objects, std::vector<Light> lights)
		:m_objects{objects}, m_lights{lights}
	{
	}
private:
	std::vector<Object> m_objects{};
	std::vector<Light> m_lights{};
};

#endif