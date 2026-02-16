#ifndef RENDER_H
#define RENDER_H

#include "Scene.h"

class Render
{
public:
	Render(const Scene& scene)
		:m_scene{scene}
	{
	}
private:
	void renderScene();
	const Scene& m_scene;
};

#endif