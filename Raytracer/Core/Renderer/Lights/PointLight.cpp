#include "PointLight.h"

#include <glm/geometric.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

PointLight::PointLight(glm::vec3 pos, glm::vec3 c, float i) : position(pos), Light(c, i) {}

void PointLight::getLightDirectionAndIntensity(glm::vec3 objectPoint, glm::vec3 & lightDirection, glm::vec3 & attenuatedLight, float & tDistance)
{
	lightDirection = objectPoint - this->position;

	//Calculate the tDistance to send back to the trace function to bound upper range for object intersection
	float distanceSquared = lightDirection.length();
	tDistance = sqrtf(distanceSquared);

	lightDirection = glm::normalize(lightDirection);

	attenuatedLight = this->intensity * this->color / (4.f * (float)M_PI * distanceSquared);
}
