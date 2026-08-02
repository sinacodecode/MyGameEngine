#ifndef CONFIGVALUES_H
#define CONFIGVALUES_H

#include "camera.h"
#include "shader_s.h"
namespace Rendering {

	extern bool isBackpackRendered;

	extern const unsigned int SCR_WIDTH;
	extern const unsigned int SCR_HEIGHT;

	extern bool isPaused;

	extern float deltaTime;
	extern float lastFrame;

	extern Camera camera;

	extern float lastX;
	extern float lastY;
	extern bool firstMouse;

	// timing
	extern float rotator;
	extern glm::vec3 lightPos;
}

namespace KeyEvents
{
	extern bool pWasPressed;

}

namespace Shaders
{
	extern float outlineScale;
}
#endif
