#ifndef RENDERER_H
#define RENDERER_H

#include "include/Scene.h"
#include "include/shader_s.h"

class Renderer
{
public:
	Renderer(Scene& scene, Shader& shader)
		:m_scene { scene }, m_shader{ shader }
	{
	}

	Scene& getScene() { return m_scene; };
	void render();
private:
	Scene& m_scene;
	Shader& m_shader;
};

#endif