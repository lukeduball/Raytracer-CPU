#include "DirectionalLight.h"

#include "glm/geometric.hpp"

DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 c, float i) : direction(glm::normalize(dir)), Light(c, i) {}

glm::vec3 DirectionalLight::getDirection()
{
	return direction;
}
