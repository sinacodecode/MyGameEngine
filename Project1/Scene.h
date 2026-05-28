#ifndef SCENE_H
#define SCENE_H

#include "SceneObject.h"
#include "Light.h"

#include <vector>

class Scene
{
public:
	Scene(Camera& camera, std::vector<Object> objects, std::vector<Light> lights)
		:m_camera{camera},m_objects {objects}, m_lights{ lights }
	{
	}
	Scene(Scene&) = delete;

	//FROM HERE !
	void popObject(int index)
	{
		m_objects.erase(m_objects.begin() + index);
	}
	void pushObject(const Object& object)
	{
		m_objects.push_back(object);
	}

	//access functions
	std::vector<Object>& getSceneObjects() { return m_objects; }
	std::vector<Light>& getSceneLights() { return m_lights; }
	Camera& getSceneCamera() { return m_camera; }
private:
	std::vector<Object> m_objects{};
	std::vector<Light> m_lights{};
	Camera& m_camera;
};

#endif