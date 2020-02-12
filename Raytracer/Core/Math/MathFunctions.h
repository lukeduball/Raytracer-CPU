#pragma once

#include <glm/vec3.hpp>

#define ARE_FLOATS_EQUAL(f1, f2) MathFunctions::areFloatsEqual(f1, f2)

class MathFunctions
{
public:
	static float EPSILON;

	static float degreesToRadians(float degrees);
	static float clamp(float min, float max, float value);
	static glm::vec3 mix(glm::vec3 a, glm::vec3 b, float mixVal);
	static bool areFloatsEqual(float f1, float f2);
};