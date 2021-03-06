#include "MathFunctions.h"

#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

constexpr float RADIAN_CONVERSION_FACTOR = (float)M_PI / 180.0f;

//Margin of error used to compare float values
const float MathFunctions::EPSILON = 0.0001f;
//Maximum value a float can be, t-value of a parametric equation of a ray when the ray has no intersection
const float MathFunctions::T_INFINITY = std::numeric_limits<float>::max();

float MathFunctions::degreesToRadians(float degrees)
{
	return degrees * RADIAN_CONVERSION_FACTOR;
}

float MathFunctions::clamp(float min, float max, float value)
{
	
	return std::max(min, std::min(max, value));
}

glm::vec3 MathFunctions::mix(glm::vec3 a, glm::vec3 b, float mixVal)
{
	return a * (1 - mixVal) + b * mixVal;
}

//Compares if two floats are equal given a margin of error
bool MathFunctions::areFloatsEqual(float f1, float f2)
{
	return fabsf(f1 - f2) < EPSILON;
}
