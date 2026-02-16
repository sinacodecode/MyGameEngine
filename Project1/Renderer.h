#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
public:
	Renderer(const Object& object, const Light& light)
	{
		draw(object, light)
	}
};

#endif