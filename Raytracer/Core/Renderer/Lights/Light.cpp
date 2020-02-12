#include "Light.h"

Light::Light(glm::vec3 c, float i) : color(c), intensity(i) {}

float Light::getIntensity()
{
	return intensity;
}

glm::vec3 Light::getColor()
{
	return color;
}
