#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 dir, glm::vec3 c, float i);

	glm::vec3 getDirection();

private:
	glm::vec3 direction;
};