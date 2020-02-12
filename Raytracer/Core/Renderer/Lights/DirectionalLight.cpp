#include "DirectionalLight.h"

#include <glm/geometric.hpp>

#include "../../Math/MathFunctions.h"

DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 c, float i) : direction(glm::normalize(dir)), Light(c, i) {}

void DirectionalLight::getLightDirectionAndIntensity(glm::vec3 objectPoint, glm::vec3 & lightDirection, glm::vec3 & attenuatedLight, float & tDistance)
{
	lightDirection = direction;
	tDistance = MathFunctions::T_INFINITY;
	attenuatedLight = intensity * color;
}

glm::vec3 DirectionalLight::getDirection()
{
	return direction;
}
