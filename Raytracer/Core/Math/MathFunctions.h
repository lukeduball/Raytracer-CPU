#pragma once

#include <glm/vec3.hpp>

class MathFunctions
{
public:
	static float degreesToRadians(float degrees);
	static float clamp(float min, float max, float value);
	static glm::vec3 mix(glm::vec3 a, glm::vec3 b, float mixVal);
};