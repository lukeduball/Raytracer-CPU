#pragma once

#include "Object.h"
#include "../Renderer/Ray.h"

class Sphere : public Object
{
public:
	Sphere(glm::vec3 pos, float rad, glm::vec3 color);

	glm::vec3 getNormalData(glm::vec3 &intersectionPoint);
	glm::vec2 getTextureCoordData(glm::vec3 &intersectionPoint, glm::vec3 &normal);

	bool intersect(const Ray & ray, float & parameter);

private:
	float radius;
	float radiusSquared;
};