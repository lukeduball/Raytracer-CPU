#include "MathFunctions.h"

#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

const float MathFunctions::EPSILON = 0.0001;
const float MathFunctions::T_INFINITY = std::numeric_limits<float>::max();

float MathFunctions::degreesToRadians(float degrees)
{
	return degrees * (float)M_PI / 180.0f;
}

float MathFunctions::clamp(float min, float max, float value)
{
	
	return std::max(min, std::min(max, value));
}

glm::vec3 MathFunctions::mix(glm::vec3 a, glm::vec3 b, float mixVal)
{
	return a * (1 - mixVal) + b * mixVal;
}

bool MathFunctions::areFloatsEqual(float f1, float f2)
{
	return fabsf(f1 - f2) < EPSILON;
}
