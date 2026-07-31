#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace Transformations
{
	void translate(glm::mat4& model ,const glm::vec3& pos)
	{
		model = glm::translate(model, pos);
	}
	void rotate(glm::mat4& model ,float angle = 0.0f ,const glm::vec3 axis = { 1.0f, 0.0f, 0.0f })
	{
		if (glm::length2(axis) > 0.00001f)
		{
			glm::quat q = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
			model = model * glm::mat4_cast(q);
		}
	}
	void rotateEuler(glm::mat4& model ,float angle = 0.0f ,const glm::vec3 axis = { 0.0f, 0.0f, 1.0f })
	{
		model = glm::rotate(model, angle, axis);
	}
	void scale(glm::mat4& model ,const glm::vec3 scale)
	{
		model = glm::scale(model, scale);
	}
}

#endif