#include "MathFunctions.h"

#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

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
