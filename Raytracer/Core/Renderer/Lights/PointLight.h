#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(glm::vec3 pos, glm::vec3 c, float i);

	void getLightDirectionAndIntensity(glm::vec3 objectPoint, glm::vec3 &lightDirection, glm::vec3 & attenuatedLight, float & tDistance);

private:
	glm::vec3 position;
};