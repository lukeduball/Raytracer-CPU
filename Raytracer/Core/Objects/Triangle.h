#pragma once

#include "Object.h"

class Triangle: public Object
{
public:
	Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, Material * material);

	bool intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData);

	void getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords);

	static bool intersectTriangle(const Ray & ray, const glm::vec3 & vertex1, const glm::vec3 & vertex2, const glm::vec3 & vertex3, float & parameter);

private:
	glm::vec3 vertex1;
	glm::vec3 vertex2;
	glm::vec3 vertex3;
};