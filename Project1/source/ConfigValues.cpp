#include "include/ConfigValues.h"

namespace Rendering {
	extern bool isBackpackRendered = false;

	const unsigned int SCR_WIDTH = 1280;
	const unsigned int SCR_HEIGHT = 720;

	extern bool isPaused = false;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	Camera camera = (glm::vec3(0.0f, 0.0f, 3.0f));

	float lastX = Rendering::SCR_WIDTH / 2.0f;
	float lastY = Rendering::SCR_HEIGHT / 2.0f;
	bool firstMouse = true;

	// timing
	float rotator{};
	glm::vec3 lightPos = { 2.0f, 2.0f, 1.5f };

	int selectedObject{};
}

namespace KeyEvents
{
	bool pWasPressed = false;

}

namespace Shaders
{
	float outlineScale = 1.03f;
}