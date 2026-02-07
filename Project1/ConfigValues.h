#ifndef CONFIGVALUES_H
#define CONFIGVALUES_H

#include "camera.h"

namespace Rendering {
	extern const unsigned int SCR_WIDTH;
	extern const unsigned int SCR_HEIGHT;

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

#endif
