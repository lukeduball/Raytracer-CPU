#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 dir, glm::vec3 c, float i);

	void getLightDirectionAndIntensity(const glm::vec3 & objectPoint, glm::vec3 & lightDirection, glm::vec3 & attenuatedLight, float & tDistance);

	glm::vec3 getDirection();

private:
	glm::vec3 direction;
};