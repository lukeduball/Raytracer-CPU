#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(glm::vec3 pos, glm::vec3 c, float i);

private:
	glm::vec3 position;
};