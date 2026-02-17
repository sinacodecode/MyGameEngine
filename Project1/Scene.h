#ifndef SCENE_H
#define SCENE_H

#include "SceneObject.h"
#include "Light.h"

#include <vector>

class Scene
{
public:
	Scene(Camera camera, std::vector<Object> objects, std::vector<Light> lights)
		:m_camera{camera},m_objects {objects}, m_lights{ lights }
	{
	}
	Scene(Scene&) = delete;

	std::vector<Object> getSceneObjects() const { return m_objects; }
	std::vector<Light> getSceneLights() const { return m_lights; }
	Camera& getSceneCamera() { return m_camera; }
private:
	std::vector<Object> m_objects{};
	std::vector<Light> m_lights{};
	Camera& m_camera;
};

#endif