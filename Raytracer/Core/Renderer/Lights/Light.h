#pragma once

#include <glm/vec3.hpp>

class Light
{
public:
	Light(glm::vec3 c, float i);

	float getIntensity();
	glm::vec3 getColor();

private:
	glm::vec3 color;
	float intensity;
};