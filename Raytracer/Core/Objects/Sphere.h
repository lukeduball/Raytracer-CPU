#pragma once

#include "Object.h"
#include "../Renderer/Ray.h"

class Sphere : public Object
{
public:
	Sphere(glm::vec3 pos, float rad, glm::vec3 color);

	void getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords);

	bool intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData);

private:
	float radius;
	float radiusSquared;
};