#pragma once

#include <glm/vec3.hpp>

class Light
{
public:
	Light(glm::vec3 c, float i);

	virtual void getLightDirectionAndIntensity(const glm::vec3 & objectPoint, glm::vec3 & lightDirection, glm::vec3 & attenuatedLight, float & tDistance) = 0;

	float getIntensity();
	glm::vec3 getColor();

protected:
	glm::vec3 color;
	float intensity;
};