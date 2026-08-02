#ifndef RENDERER_H
#define RENDERER_H

#include "include/Scene.h"
#include "include/shader_s.h"

class Renderer
{
public:

	Renderer(Renderer&) = delete;
	Renderer& operator = (Renderer&) = delete;

	Renderer(std::unique_ptr<Scene> scene, Shader& shader)
		:m_scene { std::move(scene) }, m_shader{ shader }
	{
	}

	std::unique_ptr<Scene>& getScene() { return m_scene; };
	void render();
private:
	std::unique_ptr<Scene> m_scene;
	Shader& m_shader;

	std::unique_ptr<Shader> outlineShader = std::make_unique<Shader>("../Resources/outLine.vs", "../Resources/outLine.fs");
};

#endif